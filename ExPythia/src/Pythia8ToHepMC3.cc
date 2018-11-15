#include "../interfaces/pythia8/include/Pythia8/Pythia8ToHepMC3.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/FourVector.h"

#include <deque>
#include <cassert>

namespace HepMC {

/** What is not in current HepMC implementation:
 *  - color flow (will probably be removed altogether)
 *  - beam particles
 *  - random seeds 
 *  - mpi
 */
bool Pythia8ToHepMC3::fill_next_event( Pythia8::Event& pyev, GenEvent* evt, int ievnum, Pythia8::Info* pyinfo, Pythia8::Settings* pyset) {

    // 1. Error if no event passed.
    if (!evt) {
        std::cerr << "Pythia8ToHepMC::fill_next_event error - passed null event."
                  << std::endl;
        return 0;
    }

    // Event number counter.
    if ( ievnum >= 0 ) {
        evt->set_event_number(ievnum);
        m_internal_event_number = ievnum;
    }
    else {
        evt->set_event_number(m_internal_event_number);
        ++m_internal_event_number;
    }

    evt->set_units(HepMC::Units::GEV,HepMC::Units::MM);

    // 2. Fill particle information
    std::vector<GenParticlePtr> hepevt_particles;
    hepevt_particles.reserve( pyev.size() );

    for(int i=0;i<pyev.size(); ++i) {
        hepevt_particles.push_back( make_shared<GenParticle>( FourVector( pyev[i].px(), pyev[i].py(),
                                                              pyev[i].pz(), pyev[i].e() ),
                                                              pyev[i].id(), pyev[i].statusHepMC() )
                                  );
/*
        hepevt_particles[i]->suggest_barcode(i);
*/
        hepevt_particles[i]->set_generated_mass( pyev[i].m() );
    }

    // 3. Fill vertex information
    std::vector<GenVertexPtr> vertex_cache;

    for(int i=1;i<pyev.size(); ++i) {

        std::vector<int> mothers = pyev[i].motherList();

        if(mothers.size()) {
            GenVertexPtr prod_vtx = hepevt_particles[mothers[0]]->end_vertex();

            if(!prod_vtx) {
                prod_vtx = make_shared<GenVertex>();
                vertex_cache.push_back(prod_vtx);

                for(unsigned int j=0; j<mothers.size(); ++j) {
                    prod_vtx->add_particle_in( hepevt_particles[mothers[j]] );
                }
            }
            FourVector prod_pos( pyev[i].xProd(), pyev[i].yProd(),
                                 pyev[i].zProd(), pyev[i].tProd() );

            // Update vertex position if necessary
            if(!prod_pos.is_zero() && prod_vtx->position().is_zero()) prod_vtx->set_position( prod_pos );

            prod_vtx->add_particle_out( hepevt_particles[i] );
        }
    }

    // Reserve memory for the event
    evt->reserve( hepevt_particles.size(), vertex_cache.size() );

    // Here we assume that the first two particles are the beam particles
    vector<GenParticlePtr> beam_particles;
    beam_particles.push_back(hepevt_particles[0]);
    beam_particles.push_back(hepevt_particles[1]);

    // Add particles and vertices in topological order
    evt->add_tree( beam_particles );
    //Attributes should be set after adding the particles to event
    for(int i=0;i<pyev.size(); ++i) {
        /* TODO: Set polarization */
        // Colour flow uses index 1 and 2.
        int colType = pyev[i].colType();
        if (colType ==  -1 ||colType ==  1 || colType == 2)
        {
        int flow1=0, flow2=0;
        if (colType ==  1 || colType == 2) flow1=pyev[i].col();
        if (colType == -1 || colType == 2) flow2=pyev[i].acol();
        hepevt_particles[i]->add_attribute("flow1",make_shared<IntAttribute>(flow1));
        hepevt_particles[i]->add_attribute("flow2",make_shared<IntAttribute>(flow2));
        }
     }

/*
    evt->set_beam_particles( hepevt_particles[1], hepevt_particles[2] );
*/

    // If hadronization switched on then no final coloured particles.
    bool doHadr = (pyset == 0) ? m_free_parton_warnings : pyset->flag("HadronLevel:all") && pyset->flag("HadronLevel:Hadronize");

    // 4. Check for particles which come from nowhere, i.e. are without
    // mothers or daughters. These need to be attached to a vertex, or else
    // they will never become part of the event.
    for (int i = 1; i < pyev.size(); ++i) {

        // Check for particles not added to the event
        // NOTE: We have to check if this step makes any sense in HepMC event standard
        if ( !hepevt_particles[i] ) {
            std::cerr << "hanging particle " << i << std::endl;

            GenVertexPtr prod_vtx;

            prod_vtx->add_particle_out( hepevt_particles[i] );
            evt->add_vertex(prod_vtx);
        }

        // Also check for free partons (= gluons and quarks; not diquarks?).
        if ( doHadr && m_free_parton_warnings ) {
            if ( hepevt_particles[i]->pid() == 21 && !hepevt_particles[i]->end_vertex() ) {
                std::cerr << "gluon without end vertex " << i << std::endl;
                if ( m_crash_on_problem ) exit(1);
            }
            if ( abs(hepevt_particles[i]->pid()) <= 6 && !hepevt_particles[i]->end_vertex() ) {
                std::cerr << "quark without end vertex " << i << std::endl;
                if ( m_crash_on_problem ) exit(1);
            }
        }
    }


    // 5. Store PDF, weight, cross section and other event information.
    // Flavours of incoming partons.
    if (m_store_pdf && pyinfo != 0) {
        int id1pdf = pyinfo->id1pdf();
        int id2pdf = pyinfo->id2pdf();
        if ( m_convert_gluon_to_0 ) {
            if (id1pdf == 21) id1pdf = 0;
            if (id2pdf == 21) id2pdf = 0;
        }

        HepMC::GenPdfInfoPtr pdfinfo = make_shared<HepMC::GenPdfInfo>();
        pdfinfo->set(id1pdf, id2pdf, pyinfo->x1pdf(),
                     pyinfo->x2pdf(), pyinfo->QFac(), pyinfo->pdf1(), pyinfo->pdf2() );
        // Store PDF information.
        evt->set_pdf_info( pdfinfo );
    }

    // Store process code, scale, alpha_em, alpha_s.
    if (m_store_proc && pyinfo != 0) {
        evt->add_attribute("signal_process_id",make_shared<IntAttribute>( pyinfo->code()));
        evt->add_attribute("event_scale",make_shared<DoubleAttribute>(pyinfo->QRen()));
        evt->add_attribute("alphaQCD",make_shared<DoubleAttribute>(pyinfo->alphaS()));
        evt->add_attribute("alphaQED",make_shared<DoubleAttribute>(pyinfo->alphaEM()));
    }

    // Store cross-section information in pb.
    if (m_store_xsec && pyinfo != 0) {
        HepMC::GenCrossSectionPtr xsec = make_shared<HepMC::GenCrossSection>();
        xsec->set_cross_section( pyinfo->sigmaGen() * 1e9, pyinfo->sigmaErr() * 1e9);
        evt->set_cross_section(xsec);
    }

    // Store event weights.
    if (m_store_weights && pyinfo != 0) {
        for (int iweight=0; iweight < pyinfo->nWeights(); ++iweight) {
            evt->weights().push_back(pyinfo->weight(iweight));
        }
    }

    // Done.
    return true;
}

} // end namespace HepMC

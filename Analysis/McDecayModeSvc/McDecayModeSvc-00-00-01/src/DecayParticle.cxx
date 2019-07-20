#include "McDecayModeSvc/PartId2Name.h"
#include "McDecayModeSvc/DecayParticle.h"

//int nnnnnn_instance = 0;

DecayParticle::DecayParticle(int id)
  : m_id(id)
{
  //std::cout << "new, left " << ++nnnnnn_instance << std::endl;
}

DecayParticle::~DecayParticle()
{
  if ( ! m_daughters.empty() ) {
    std::list<DecayParticle*>::iterator it = m_daughters.begin();
    for ( ; it != m_daughters.end(); it++) delete (*it);
  }
  //std::cout << "delete, left " << --nnnnnn_instance << std::endl;
}

DecayParticle* DecayParticle::addDaughter(int id) {

  std::list<DecayParticle*>::iterator it = m_daughters.begin();
  std::list<DecayParticle*>::iterator end = m_daughters.end();

  while(it != end && (*it)->id() < id) it++;

  DecayParticle* daughter = new DecayParticle(id);
  m_daughters.insert(it, daughter);

  return daughter;
}

bool DecayParticle::sameAs(DecayParticle* other) const {

  if (this->m_id == other->m_id) {

    if (this->m_daughters.size() == other->m_daughters.size()) {

      std::list<DecayParticle*>::const_iterator iti = m_daughters.begin();
      std::list<DecayParticle*>::const_iterator itj = other->m_daughters.begin();

      for ( ; iti != m_daughters.end() ; iti++, itj++) {
	if (! (*iti)->sameAs(*itj) ) return false; 
      }

      return true;
    }
  }

  return false;
}

void DecayParticle::printToStream(std::ostream& os, int depth) const {

  PartId2Name& pName = *(PartId2Name::instance());

  if (! m_daughters.empty() ) {

    for (int i = 0; i < depth; i++) os << '\t';

    os << pName(m_id) << " ->";

    std::list<DecayParticle*>::const_iterator begin = m_daughters.begin();
    std::list<DecayParticle*>::const_iterator end = m_daughters.end();
    std::list<DecayParticle*>::const_iterator it;
    for (it = begin; it != end; it++) os << ' ' << pName((*it)->id());

    os << std::endl;

    for (it = begin; it != end; it++) (*it)->printToStream(os, depth+1);
  }
}

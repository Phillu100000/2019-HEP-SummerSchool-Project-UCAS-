#ifndef DECAY_PARTICLE_H
#define DECAY_PARTICLE_H

#include <iostream>
#include <list>

class DecayParticle
{
public:
  DecayParticle(int id);
  virtual ~DecayParticle();

  int id() const { return m_id; }

  bool isLeaf() const { return m_daughters.empty(); }

  DecayParticle* addDaughter(int id);

  bool sameAs(DecayParticle* other) const;

  void printToStream(std::ostream& os, int depth = 1) const;

private:
  int m_id;

  std::list<DecayParticle*> m_daughters;
};

#endif

#include "GenePigment.h"

#include "Random.h"
#include "Swimmer.h"

#include <QPixmap>
#include <QPainter>

GenePigment::GenePigment()
    : GenePigment(Random::Number(0.0, 1.0), Random::Number(0.0, 1.0), Random::Number(0.0, 1.0), Random::Number(0.0, 1.0), Random::Number(0.0, 100.0), Random::Number(0.0, 0.2))
{
}

GenePigment::GenePigment(double a, double r, double g, double b, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , a_(std::clamp(a, 0.0, 1.0))
    , r_(std::clamp(r, 0.0, 1.0))
    , g_(std::clamp(g, 0.0, 1.0))
    , b_(std::clamp(b, 0.0, 1.0))
{
    // Mutate Alpha
    AddMutation(BASE_WEIGHT, [&]()
    {
        a_ = std::clamp(a_ + Random::Gaussian(0.0, 0.1), 0.0, 1.0);
    });

    // Mutate Red
    AddMutation(BASE_WEIGHT, [&]()
    {
        r_ = std::clamp(r_ + Random::Gaussian(0.0, 0.1), 0.0, 1.0);
    });
    // Mutate Green
    AddMutation(BASE_WEIGHT, [&]()
    {
        g_ = std::clamp(g_ + Random::Gaussian(0.0, 0.1), 0.0, 1.0);
    });
    // Mutate Blue
    AddMutation(BASE_WEIGHT, [&]()
    {
        b_ = std::clamp(b_ + Random::Gaussian(0.0, 0.1), 0.0, 1.0);
    });

    // Mutate all
    AddMutation(0.25 * BASE_WEIGHT, [&]()
    {
        a_ = std::clamp(a_ + Random::Gaussian(0.0, 0.1), 0.0, 1.0);
        r_ = std::clamp(r_ + Random::Gaussian(0.0, 0.1), 0.0, 1.0);
        g_ = std::clamp(g_ + Random::Gaussian(0.0, 0.1), 0.0, 1.0);
        b_ = std::clamp(b_ + Random::Gaussian(0.0, 0.1), 0.0, 1.0);
    });

    // Randomise colour
    AddMutation(0.01 * BASE_WEIGHT, [&]()
    {
        a_ = Random::Number(0.0, 1.0);
        r_ = Random::Number(0.0, 1.0);
        g_ = Random::Number(0.0, 1.0);
        b_ = Random::Number(0.0, 1.0);
    });
}

GenePigment::~GenePigment()
{
}

std::shared_ptr<Gene> GenePigment::Copy() const
{
    return std::make_shared<GenePigment>(a_, r_, g_, b_, GetDominance(), GetMutationProbability());
}

void GenePigment::ExpressGene(Swimmer& /*owner*/, Phenotype& target) const
{
    QPixmap canvas(1, 1);
    QPainter p(&canvas);

    p.setPen(Qt::NoPen);
    p.fillRect(canvas.rect(), Qt::white);
    p.fillRect(canvas.rect(), target.colour);
    p.fillRect(canvas.rect(), QColor::fromRgbF(r_, g_, b_, a_));

    target.colour = canvas.toImage().pixel(0, 0);
}

#include "Material.h"
#include "Random.h"

bool Lambertian::Scatter(const ray_t& ray, const raycastHit_t& raycasHit, color3_t& attenuation, ray_t& scatter)
{
    scatter.origin = raycasHit.point;
    scatter.direction = raycasHit.normal + randomOnUnitSphere();

    attenuation = m_albedo;

    return true;
}

//#include "core/integral-tool/directLightEvaluator.h"
//
//#include "core/integral-tool/sample/bsdfSample.h"
//#include "core/integral-tool/sample/directLightSample.h"
//#include "core/integral-tool/tMis.h"
//#include "core/intersector/primitive/primitive.h"
//#include "core/light/category/areaLight.h"
//#include "core/ray.h"
//#include "core/scene.h"
//#include "core/surface/bsdf/bsdf.h"
//#include "core/surface/transportInfo.h"
//#include "core/surfaceIntersection.h"
//#include "fundamental/assertion.h"
//#include "math/constant.h"
//
//namespace cadise
//{
//
//Spectrum DirectLightEvaluator::evaluate(
//    const Scene&               scene, 
//    const SurfaceIntersection& surfaceIntersection,
//    const Bsdf*                bsdf, 
//    const Light*               light) 
//{
//    CS_ASSERT(bsdf);
//    CS_ASSERT(light);
//
//    SurfaceIntersection intersection(surfaceIntersection);
//    Spectrum directLightRadiance(0.0_r);
//
//    const Vector3R P  = intersection.surfaceDetail().position();
//    const Vector3R Ns = intersection.surfaceDetail().shadingNormal();
//    const Vector3R Ng = intersection.surfaceDetail().geometryNormal();
//
//    if (bsdf->lobes().hasExactly({ ELobe::ABSORB })) 
//    {
//        return directLightRadiance;
//    }
//
//    // mis using light sampling
//    {
//        DirectLightSample directLightSample;
//        directLightSample.setTargetPosition(P);
//
//        light->evaluateDirectSample(&directLightSample);
//        if (directLightSample.isValid()) 
//        {
//            const Vector3R LVector  = directLightSample.emitPosition().sub(P);
//            const real     distance = LVector.length();
//
//            CS_ASSERT_GT(distance, 0.0_r);
//
//            const Vector3R L = LVector.div(distance);
//            intersection.setWo(L);
//
//            // generate shadow ray to do occluded test
//            Ray shadowRay(P, L);
//            shadowRay.setMaxT(distance - constant::ray_epsilon<real>);
//
//            if (!scene.isOccluded(shadowRay)) 
//            {
//                const Spectrum& radiance  = directLightSample.radiance();
//                const real      lightPdfW = directLightSample.pdfW();
//
//                const Spectrum reflectance       = bsdf->evaluate(TransportInfo(), intersection);
//                const Spectrum directLightFactor = reflectance.mul(L.absDot(Ns) / lightPdfW);
//
//                // if light is delta light, not using mis technique
//                if (light->isDeltaLight()) 
//                {
//                    directLightRadiance.addLocal(radiance.mul(directLightFactor));
//                }
//                else
//                {
//                    const real bsdfPdfW  = bsdf->evaluatePdfW(TransportInfo(), intersection);
//                    const real misWeight = TMis<EMisMode::POWER>().weight(lightPdfW, bsdfPdfW);
//
//                    directLightRadiance.addLocal(radiance.mul(directLightFactor).mul(misWeight));
//                }
//            }
//        }
//    } // mis using light sampling
//
//    // mis using bsdf sampling
//    {
//        if (!light->isDeltaLight())
//        {
//            BsdfSample bsdfSample;
//            bsdf->evaluateSample(TransportInfo(), intersection, &bsdfSample);
//            if (bsdfSample.isValid())
//            {
//                const Spectrum& reflectance = bsdfSample.scatterValue();
//                const Vector3R& L           = bsdfSample.scatterDirection();
//
//                CS_ASSERT(!L.isZero());
//
//                Ray sampleRay(P, L);
//                SurfaceIntersection localIntersection;
//                if (scene.isIntersecting(sampleRay, localIntersection))
//                {
//                    const AreaLight* areaLight = localIntersection.primitiveInfo().primitive()->areaLight();
//                    if (areaLight == light) 
//                    {
//                        const real bsdfPdfW  = bsdfSample.pdfW();
//                        const real lightPdfW = areaLight->evaluateDirectPdfW(localIntersection, P);
//                        const real misWeight = TMis<EMisMode::POWER>().weight(bsdfPdfW, lightPdfW);
//
//                        const Spectrum directLightFactor = reflectance.mul(L.absDot(Ns) / bsdfPdfW);
//                        const Spectrum radiance          = areaLight->emittance(localIntersection);
//
//                        directLightRadiance.addLocal(radiance.mul(directLightFactor).mul(misWeight));
//                    }
//                }
//            }
//        }
//    } // mis using bsdf sampling
//
//    return directLightRadiance;
//}
//
//} // namespace cadise
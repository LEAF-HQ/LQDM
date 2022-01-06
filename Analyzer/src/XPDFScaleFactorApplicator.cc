#include "LQDM/Analyzer/include/XPDFScaleFactorApplicator.h"

using namespace std;



bool XPDFScaleFactorApplicator::process(RecoEvent & event){
  if(event.is_data) return true;

  double xpdf1 = m_pdf->xfxQ(event.geninfo->id1(), event.geninfo->x1(), event.geninfo->scale_pdf());
  double xpdf2 = m_pdf->xfxQ(event.geninfo->id2(), event.geninfo->x2(), event.geninfo->scale_pdf());
  double product = xpdf1 * xpdf2;

  set_bin(product);
  double sf = get_scalefactor();
  event.weight *= sf;

  return true;
}

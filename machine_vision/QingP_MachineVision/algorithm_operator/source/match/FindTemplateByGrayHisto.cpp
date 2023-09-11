#include "FindTemplateByGrayHisto.h"

#include "../../../common_tool/source/normal_tool/NormalTool.h"
#include "../../../common_tool/source/convert_image/ConvertImage.h"
#include "../../../common_tool/source/common_transform/CvAffineTransform.h"
#include "../../../common_tool/source/common_measure/OneDimensionMeasure.h"

FindTemplateByGrayHisto::FindTemplateByGrayHisto()
{

}

FindTemplateByGrayHisto::~FindTemplateByGrayHisto()
{

}

void FindTemplateByGrayHisto::setFindParam(const FindTemplParam& stParam)
{
	m_stFindTemplParam.cvrTemplRect = stParam.cvrTemplRect;
	m_stFindTemplParam.dScaleRatio = stParam.dScaleRatio;
	m_stFindTemplParam.bIsTemplAvaliable = stParam.bIsTemplAvaliable;
	m_stFindTemplParam.hvTemplAbsolutionHisto = stParam.hvTemplAbsolutionHisto;
	m_stFindTemplParam.hvTemplRelativeHisto = stParam.hvTemplRelativeHisto;
	m_stFindTemplParam.cvrSearchRect = stParam.cvrSearchRect;
	m_stFindTemplParam.dScoreThres = stParam.dScoreThres;
	m_stFindTemplParam.dFindRowStep = stParam.dFindRowStep;
	m_stFindTemplParam.dFindColStep = stParam.dFindColStep;
}

void FindTemplateByGrayHisto::getFindParam(FindTemplParam& stParam)
{
	stParam.cvrTemplRect = m_stFindTemplParam.cvrTemplRect;
	stParam.dScaleRatio = m_stFindTemplParam.dScaleRatio;
	stParam.bIsTemplAvaliable = m_stFindTemplParam.bIsTemplAvaliable;
	stParam.hvTemplAbsolutionHisto = m_stFindTemplParam.hvTemplAbsolutionHisto;
	stParam.hvTemplRelativeHisto = m_stFindTemplParam.hvTemplRelativeHisto;
	stParam.cvrSearchRect = m_stFindTemplParam.cvrSearchRect;
	stParam.dScoreThres = m_stFindTemplParam.dScoreThres;
	stParam.dFindRowStep = m_stFindTemplParam.dFindRowStep;
	stParam.dFindColStep = m_stFindTemplParam.dFindColStep;
}

void FindTemplateByGrayHisto::createTempl(cv::Mat cvmImage)
{
    try
    {
        HalconCpp::HObject hoImage = CommonTool_ConvertImage::Mat2HObject(cvmImage);
        HalconCpp::HTuple hvIsOK;
        ggsCreateTemplByHisto(hoImage,
            m_stFindTemplParam.cvrTemplRect.tl().y, m_stFindTemplParam.cvrTemplRect.tl().x,
            m_stFindTemplParam.cvrTemplRect.br().y - 1, m_stFindTemplParam.cvrTemplRect.br().x - 1,
            m_stFindTemplParam.dScaleRatio,
            &hvIsOK, &m_stFindTemplParam.hvTemplAbsolutionHisto, &m_stFindTemplParam.hvTemplRelativeHisto);
        if (0 == hvIsOK.I())
        {
            m_stFindTemplParam.bIsTemplAvaliable = false;
        }
        else
        {
            m_stFindTemplParam.bIsTemplAvaliable = true;
        }
    }
    catch (HalconCpp::HException& ex)
    {
        m_stFindTemplParam.bIsTemplAvaliable = false;
    }
}

void FindTemplateByGrayHisto::findTempl(cv::Mat cvmImage, FindTemplResult& stResult)
{
    try
    {
        stResult.bIsMatchOK = false;
        if (false == m_stFindTemplParam.bIsTemplAvaliable)
        {
            return;
        }

        HalconCpp::HTuple hvIsOK, hvFindRows, hvFindCols, hvFindScores;
        HalconCpp::HObject hoImage = CommonTool_ConvertImage::Mat2HObject(cvmImage);
        ggsFindTemplByHisto(hoImage,
            m_stFindTemplParam.cvrSearchRect.tl().y, m_stFindTemplParam.cvrSearchRect.tl().x,
            m_stFindTemplParam.cvrSearchRect.br().y - 1, m_stFindTemplParam.cvrSearchRect.br().x - 1,
            m_stFindTemplParam.dScoreThres, m_stFindTemplParam.hvTemplAbsolutionHisto, m_stFindTemplParam.hvTemplRelativeHisto,
            m_stFindTemplParam.cvrTemplRect.width, m_stFindTemplParam.cvrTemplRect.height,
            m_stFindTemplParam.dFindRowStep, m_stFindTemplParam.dFindColStep, m_stFindTemplParam.dScaleRatio,
            &hvIsOK, &hvFindRows, &hvFindCols, &hvFindScores);
        if (0 == hvIsOK.I())
        {
            return;
        }
        stResult.bIsMatchOK = true;
        for (int iIdx = 0; iIdx < hvFindRows.Length(); ++iIdx)
        {
            FindTemplResult::MatchResult stMatchResult;
            stMatchResult.dMatchScore = hvFindScores[iIdx].D();
            stMatchResult.cvpMatchCenter.x = hvFindCols[iIdx].D();
            stMatchResult.cvpMatchCenter.y = hvFindRows[iIdx].D();
            stResult.lststMatchResult.append(stMatchResult);
        }
    }
    catch (HalconCpp::HException& ex)
    {
        stResult.bIsMatchOK = false;
    }
    catch (std::exception& ex)
    {
        stResult.bIsMatchOK = false;
    }
}

void FindTemplateByGrayHisto::ggsCreateTemplByHisto(HalconCpp::HObject ho_Image, HalconCpp::HTuple hv_templRow1, HalconCpp::HTuple hv_templCol1,
    HalconCpp::HTuple hv_templRow2, HalconCpp::HTuple hv_templCol2, HalconCpp::HTuple hv_scaleRatio, HalconCpp::HTuple* hv_isOK,
    HalconCpp::HTuple* hv_templAbsolutionHisto, HalconCpp::HTuple* hv_templRelativeHisto)
{
    using namespace HalconCpp;

    // Local iconic variables
    HObject  ho_templRect, ho_ImageZoomed;

    // Local control variables
    HTuple  hv_Exception;

    try
    {
        (*hv_isOK) = 0;
        (*hv_templAbsolutionHisto) = HTuple();
        (*hv_templRelativeHisto) = HTuple();

        GenRectangle1(&ho_templRect, hv_templRow1 * hv_scaleRatio, hv_templCol1 * hv_scaleRatio,
            hv_templRow2 * hv_scaleRatio, hv_templCol2 * hv_scaleRatio);
        ZoomImageFactor(ho_Image, &ho_ImageZoomed, hv_scaleRatio, hv_scaleRatio, "constant");
        GrayHisto(ho_templRect, ho_ImageZoomed, &(*hv_templAbsolutionHisto), &(*hv_templRelativeHisto));
        (*hv_isOK) = 1;
    }
    // catch (Exception) 
    catch (HException& HDevExpDefaultException)
    {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
        (*hv_isOK) = 0;
        (*hv_templAbsolutionHisto) = HTuple();
        (*hv_templRelativeHisto) = HTuple();
    }
    return;
}

void FindTemplateByGrayHisto::ggsFindTemplByHisto(HalconCpp::HObject ho_Image, HalconCpp::HTuple hv_searchRow1, HalconCpp::HTuple hv_searchCol1,
    HalconCpp::HTuple hv_searchRow2, HalconCpp::HTuple hv_searchCol2, HalconCpp::HTuple hv_scoreThres, HalconCpp::HTuple hv_templAbsolutionHisto,
    HalconCpp::HTuple hv_templRelativeHisto, HalconCpp::HTuple hv_maskWidth, HalconCpp::HTuple hv_maskHeight, HalconCpp::HTuple hv_rowStep,
    HalconCpp::HTuple hv_colStep, HalconCpp::HTuple hv_scaleRatio, HalconCpp::HTuple* hv_isOK, HalconCpp::HTuple* hv_findRows,
    HalconCpp::HTuple* hv_findCols, HalconCpp::HTuple* hv_findScores)
{
    using namespace HalconCpp;

    // Local iconic variables
    HObject  ho_ImageZoomed, ho_rectRegion;

    // Local control variables
    HTuple  hv_curRow, hv_curCol, hv_curAbsolutionHisto;
    HTuple  hv_curRelativeHisto, hv_isAbsOK, hv_absCosSimilarity;
    HTuple  hv_isRelOK, hv_relCosSimilarity, hv_curSimilarity;
    HTuple  hv_diffGrayHisto, hv_diffRatio, hv_Exception, hv_sortIndices;

    try
    {
        (*hv_isOK) = 0;
        (*hv_findRows) = HTuple();
        (*hv_findCols) = HTuple();
        (*hv_findScores) = HTuple();

        hv_searchRow1 = hv_searchRow1 * hv_scaleRatio;
        hv_searchCol1 = hv_searchCol1 * hv_scaleRatio;
        hv_searchRow2 = hv_searchRow2 * hv_scaleRatio;
        hv_searchCol2 = hv_searchCol2 * hv_scaleRatio;

        hv_maskWidth = hv_maskWidth * hv_scaleRatio;
        hv_maskHeight = hv_maskHeight * hv_scaleRatio;

        ZoomImageFactor(ho_Image, &ho_ImageZoomed, hv_scaleRatio, hv_scaleRatio, "constant");

        {
            HTuple end_val16 = hv_searchRow2 - hv_maskHeight;
            HTuple step_val16 = hv_rowStep;
            for (hv_curRow = hv_searchRow1; hv_curRow.Continue(end_val16, step_val16); hv_curRow += step_val16)
            {
                {
                    HTuple end_val17 = hv_searchCol2 - hv_maskWidth;
                    HTuple step_val17 = hv_colStep;
                    for (hv_curCol = hv_searchCol1; hv_curCol.Continue(end_val17, step_val17); hv_curCol += step_val17)
                    {
                        GenRectangle1(&ho_rectRegion, hv_curRow, hv_curCol, hv_curRow + hv_maskHeight,
                            hv_curCol + hv_maskWidth);
                        GrayHisto(ho_rectRegion, ho_ImageZoomed, &hv_curAbsolutionHisto, &hv_curRelativeHisto);
                        try
                        {
                            ggsGetCosSimilarity(hv_curAbsolutionHisto, hv_templAbsolutionHisto, &hv_isAbsOK,
                                &hv_absCosSimilarity);
                            if (0 != (int(hv_isAbsOK == 0)))
                            {
                                continue;
                            }
                            ggsGetCosSimilarity(hv_curRelativeHisto, hv_templRelativeHisto, &hv_isRelOK,
                                &hv_relCosSimilarity);
                            if (0 != (int(hv_isRelOK == 0)))
                            {
                                continue;
                            }
                            hv_curSimilarity = ((hv_absCosSimilarity * hv_relCosSimilarity).TupleAbs()).TupleSqrt();
                            hv_diffGrayHisto = (hv_templAbsolutionHisto - hv_curAbsolutionHisto).TupleAbs();
                            hv_diffRatio = ((hv_diffGrayHisto.TupleSum()).TupleReal()) / (hv_templAbsolutionHisto.TupleSum());
                            hv_curSimilarity = ((-hv_diffRatio).TupleExp()) * hv_curSimilarity;
                            if (0 != (int(hv_curSimilarity >= hv_scoreThres)))
                            {
                                (*hv_findRows) = (*hv_findRows).TupleConcat((hv_curRow + (hv_maskHeight / 2)) / hv_scaleRatio);
                                (*hv_findCols) = (*hv_findCols).TupleConcat((hv_curCol + (hv_maskWidth / 2)) / hv_scaleRatio);
                                (*hv_findScores) = (*hv_findScores).TupleConcat(hv_curSimilarity);
                            }
                        }
                        // catch (Exception) 
                        catch (HException& HDevExpDefaultException)
                        {
                            HDevExpDefaultException.ToHTuple(&hv_Exception);
                            continue;
                        }
                    }
                }
            }
        }

        TupleSortIndex((*hv_findScores), &hv_sortIndices);
        TupleInverse(hv_sortIndices, &hv_sortIndices);

        (*hv_findScores) = HTuple((*hv_findScores)[hv_sortIndices]);
        (*hv_findRows) = HTuple((*hv_findRows)[hv_sortIndices]);
        (*hv_findCols) = HTuple((*hv_findCols)[hv_sortIndices]);

        (*hv_isOK) = 1;
    }
    // catch (Exception) 
    catch (HException& HDevExpDefaultException)
    {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
        (*hv_isOK) = 0;
        (*hv_findRows) = HTuple();
        (*hv_findCols) = HTuple();
        (*hv_findScores) = HTuple();
    }

    return;
}

void FindTemplateByGrayHisto::ggsGetCosSimilarity(HalconCpp::HTuple hv_vec1, HalconCpp::HTuple hv_vec2, HalconCpp::HTuple* hv_isOK, HalconCpp::HTuple* hv_cosSimilarity)
{
    using namespace HalconCpp;

    // Local iconic variables

    // Local control variables
    HTuple  hv_dotMultValue, hv_vec1normal, hv_vec2normal;
    HTuple  hv_Exception;

    try
    {
        hv_dotMultValue = (hv_vec1 * hv_vec2).TupleSum();
        hv_vec1normal = ((hv_vec1 * hv_vec1).TupleSum()).TupleSqrt();
        hv_vec2normal = ((hv_vec2 * hv_vec2).TupleSum()).TupleSqrt();
        (*hv_cosSimilarity) = hv_dotMultValue / (hv_vec1normal * hv_vec2normal);
        (*hv_isOK) = 1;
    }
    // catch (Exception) 
    catch (HException& HDevExpDefaultException)
    {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
        (*hv_isOK) = 0;
        (*hv_cosSimilarity) = 0;
    }

    return;
}




	   
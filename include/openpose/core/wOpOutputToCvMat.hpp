#ifndef OPENPOSE__CORE__W_OP_OUTPUT_TO_CV_MAT_HPP
#define OPENPOSE__CORE__W_OP_OUTPUT_TO_CV_MAT_HPP

#include <memory> // std::shared_ptr
#include <opencv2/core/core.hpp>
#include "../thread/worker.hpp"
#include "opOutputToCvMat.hpp"

namespace op
{
    template<typename TDatums>
    class WOpOutputToCvMat : public Worker<TDatums>
    {
    public:
        explicit WOpOutputToCvMat(const std::shared_ptr<OpOutputToCvMat>& opOutputToCvMat);

        void initializationOnThread();

        void work(TDatums& tDatums);

    private:
        const std::shared_ptr<OpOutputToCvMat> spOpOutputToCvMat;

        DELETE_COPY(WOpOutputToCvMat);
    };
}





// Implementation
#include <vector>
#include "../utilities/errorAndLog.hpp"
#include "../utilities/macros.hpp"
#include "../utilities/pointerContainer.hpp"
#include "../utilities/profiler.hpp"
namespace op
{
    template<typename TDatums>
    WOpOutputToCvMat<TDatums>::WOpOutputToCvMat(const std::shared_ptr<OpOutputToCvMat>& opOutputToCvMat) :
        spOpOutputToCvMat{opOutputToCvMat}
    {
    }

    template<typename TDatums>
    void WOpOutputToCvMat<TDatums>::initializationOnThread()
    {
    }

    template<typename TDatums>
    void WOpOutputToCvMat<TDatums>::work(TDatums& tDatums)
    {
        try
        {
            if (checkNoNullNorEmpty(tDatums))
            {
                // Debugging log
                dLog("", Priority::Low, __LINE__, __FUNCTION__, __FILE__);
                // Profiling speed
                const auto profilerKey = Profiler::timerInit(__LINE__, __FUNCTION__, __FILE__);
                // float* -> cv::Mat
                for (auto& tDatum : *tDatums)
                    tDatum.cvOutputData = spOpOutputToCvMat->formatToCvMat(tDatum.outputData);
                // Profiling speed
                Profiler::timerEnd(profilerKey);
                Profiler::printAveragedTimeMsOnIterationX(profilerKey, __LINE__, __FUNCTION__, __FILE__, 1000);
                // Debugging log
                dLog("", Priority::Low, __LINE__, __FUNCTION__, __FILE__);
            }
        }
        catch (const std::exception& e)
        {
            this->stop();
            tDatums = nullptr;
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }

    COMPILE_TEMPLATE_DATUM(WOpOutputToCvMat);
}

#endif // OPENPOSE__CORE__W_OP_OUTPUT_TO_CV_MAT_HPP

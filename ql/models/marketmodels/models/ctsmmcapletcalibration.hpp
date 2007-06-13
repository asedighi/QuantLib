/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2007 Ferdinando Ametrano
 Copyright (C) 2007 Mark Joshi

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#ifndef quantlib_ctsmm_caplet_calibration_hpp
#define quantlib_ctsmm_caplet_calibration_hpp

#include <ql/models/marketmodels/curvestate.hpp>
#include <ql/models/marketmodels/evolutiondescription.hpp>
#include <ql/models/marketmodels/piecewiseconstantcorrelation.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace QuantLib {

    class PiecewiseConstantVariance;
    class Matrix;

    class CTSMMCapletCalibration {
      public:
        virtual ~CTSMMCapletCalibration();
        CTSMMCapletCalibration(
            const EvolutionDescription& evolution,
            const boost::shared_ptr<PiecewiseConstantCorrelation>& corr,
            const std::vector<boost::shared_ptr<
                        PiecewiseConstantVariance> >&
                                    displacedSwapVariances,
            const std::vector<Volatility>& mktCapletVols, // displaced??
            const boost::shared_ptr<CurveState>& cs,
            Spread displacement); // ??
        // modifiers
        bool calibrate(Natural numberOfFactors,

                       Natural maxIterations,
                       Real tolerance,

                       Natural innerMaxIterations = 100,
                       Real innerTolerance = 1e-8);
        // inspectors
        Natural failures() const;
        Real deformationSize() const;

        Real capletRmsError() const;
        Real capletMaxError() const;
        Real swaptionRmsError() const;
        Real swaptionMaxError() const;

        const std::vector<Matrix>& swapPseudoRoots() const;
        const Matrix& swapPseudoRoot(Size i) const;

        static void performChecks(
            const EvolutionDescription& evolution,
            const PiecewiseConstantCorrelation& corr,
            const std::vector<boost::shared_ptr<
                        PiecewiseConstantVariance> >&
                                    displacedSwapVariances,
            const std::vector<Volatility>& mktCapletVols_,
            const CurveState& cs);
      protected:
        virtual Natural calibrationImpl_(Natural numberOfFactors,

                                         Natural innerMaxIterations,
                                         Real innerTolerance) = 0;
        // input
        EvolutionDescription evolution_;
        boost::shared_ptr<PiecewiseConstantCorrelation> corr_;
        std::vector<boost::shared_ptr<PiecewiseConstantVariance> >
                                                displacedSwapVariances_;
        std::vector<Volatility> mktCapletVols_;
        boost::shared_ptr<CurveState> cs_;
        Spread displacement_;
        Size numberOfRates_;
        // working variables
        std::vector<Volatility> usedCapletVols_;
        // results
        bool calibrated_;
        Natural failures_;
        Real deformationSize_;
        Real capletRmsError_, capletMaxError_;
        Real swaptionRmsError_, swaptionMaxError_;
        std::vector<Matrix> swapCovariancePseudoRoots_;
    };

    // inline

    inline Natural CTSMMCapletCalibration::failures() const {
        QL_REQUIRE(calibrated_, "not successfully calibrated yet");
        return failures_;
    }

    inline Real CTSMMCapletCalibration::deformationSize() const {
        QL_REQUIRE(calibrated_, "not successfully calibrated yet");
        return deformationSize_;
    }

    inline Real CTSMMCapletCalibration::capletRmsError() const {
        QL_REQUIRE(calibrated_, "not successfully calibrated yet");
        return capletRmsError_;
    }

    inline Real CTSMMCapletCalibration::capletMaxError() const {
        QL_REQUIRE(calibrated_, "not successfully calibrated yet");
        return capletMaxError_;
    }

    inline Real CTSMMCapletCalibration::swaptionRmsError() const {
        QL_REQUIRE(calibrated_, "not successfully calibrated yet");
        return swaptionRmsError_;
    }

    inline Real CTSMMCapletCalibration::swaptionMaxError() const {
        QL_REQUIRE(calibrated_, "not successfully calibrated yet");
        return swaptionMaxError_;
    }

    inline const std::vector<Matrix>&
    CTSMMCapletCalibration::swapPseudoRoots() const {
        QL_REQUIRE(calibrated_, "not successfully calibrated yet");
        return swapCovariancePseudoRoots_;
    }

    inline const Matrix&
    CTSMMCapletCalibration::swapPseudoRoot(Size i) const {
        QL_REQUIRE(calibrated_, "not successfully calibrated yet");
        QL_REQUIRE(i<swapCovariancePseudoRoots_.size(),
                   i << "is an invalid index, must be less than "
                   << swapCovariancePseudoRoots_.size());
        return swapCovariancePseudoRoots_[i];
    }

}

#endif
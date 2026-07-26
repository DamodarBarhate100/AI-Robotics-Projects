#pragma once
#include <cstdarg>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class LogisticRegression {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        float votes[5] = { 2.074478834054 ,0.472036891217 ,-2.326731066689 ,-0.89065899029 ,0.670874331707  };
                        votes[0] += dot(x,   0.054514852513  , -2.225921255715  , 0.061634327802  , -0.38575999865  , 0.396622920075  , 1.195056059761  , 0.116279799408  , -0.613391322978 );
                        votes[1] += dot(x,   0.49803796051  , 0.681438649503  , 0.097352374608  , 2.683141777423  , -0.638776477712  , -1.823325772496  , -0.996500337011  , 0.075578884638 );
                        votes[2] += dot(x,   -0.337381374366  , 2.347675578433  , 0.115782225945  , 1.293015695027  , 1.591479486598  , 1.434928154942  , 0.755409046371  , -0.053159807626 );
                        votes[3] += dot(x,   -0.657244827144  , -0.850003268036  , -1.299996688315  , -2.208548677486  , 0.70217424544  , -1.152707994294  , -2.623084413954  , 1.769774363129 );
                        votes[4] += dot(x,   0.442073388487  , 0.046810295814  , 1.02522775996  , -1.381848796314  , -2.0515001744  , 0.346049552087  , 2.747895905187  , -1.178802117162 );
                        // return argmax of votes
                        uint8_t classIdx = 0;
                        float maxVotes = votes[0];

                        for (uint8_t i = 1; i < 5; i++) {
                            if (votes[i] > maxVotes) {
                                classIdx = i;
                                maxVotes = votes[i];
                            }
                        }

                        return classIdx;
                    }

                protected:
                    /**
                    * Compute dot product
                    */
                    float dot(float *x, ...) {
                        va_list w;
                        va_start(w, 8);
                        float dot = 0.0;

                        for (uint16_t i = 0; i < 8; i++) {
                            const float wi = va_arg(w, double);
                            dot += x[i] * wi;
                        }

                        return dot;
                    }
                };
            }
        }
    }
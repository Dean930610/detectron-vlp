#include "reduce_with_attention_op.h"

namespace caffe2 {

REGISTER_CPU_OPERATOR(ReduceWithAttention, ReduceWithAttentionOp<float, CPUContext>);
REGISTER_CPU_OPERATOR(ReduceWithAttentionGradient, ReduceWithAttentionGradientOp<float, CPUContext>);

OPERATOR_SCHEMA(ReduceWithAttention)
    .NumInputs(2, INT_MAX)
    .NumOutputs(1)
    .SetDoc(R"DOC(
Reduce the output of different iterations with attention.
)DOC")
    .Arg(
        "iter",
        "(int) default 2; number of iterations.")
    .Input(
        0,
        "A",
        "Attention tensor.")
    .Output(
        0,
        "Y",
        "Reduced tensor.");

OPERATOR_SCHEMA(ReduceWithAttentionGradient)
    .NumInputs(3, INT_MAX)
    .NumOutputs(1)
    .Input(
        0,
        "dY",
        "Input gradient for Y.")
    .Input(
        1,
        "A",
        "Attention tensor.")
    .Output(
        0,
        "dA",
        "Output gradient for attention tensor A.");

class GetReduceWithAttentionGradient : public GradientMakerBase {
  using GradientMakerBase::GradientMakerBase;
  vector<OperatorDef> GetGradientDefs() override {
    // just copy and paste the input to the output
    vector<string> inputs;
    inputs.push_back(GO(0));
    for (int i = 0; i < def_.input_size(); ++i) {
      inputs.push_back(I(i));
    }
    return SingleGradientDef("ReduceWithAttentionGradient", 
                            "", 
                            inputs, 
                            vector<string>{GI(0)});
  }
};

REGISTER_GRADIENT(ReduceWithAttention, GetReduceWithAttentionGradient);

} // namespace caffe2
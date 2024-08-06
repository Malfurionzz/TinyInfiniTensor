#include "operators/matmul.h"
#include <cassert>
#include <cstddef>

namespace infini
{

    MatmulObj::MatmulObj(GraphObj *graph, Tensor A, Tensor B, Tensor C, bool transA,
                         bool transB)
        : OperatorObj(OpType::MatMul, TensorVec{A, B}, {C}),
          transA(transA), transB(transB)
    {
        IT_ASSERT(checkValid(graph));
    }

    string MatmulObj::toString() const
    {
        std::ostringstream os;
        os << "Matmul([" << (transA ? "A^T" : "A") << "," << (transB ? "B^T" : "B]")
           << ",A=" << inputs[0]->getGuid()
           << ",B=" << inputs[1]->getGuid() << ",C=" << outputs[0]->getGuid()
           << ",mnk=[" << m << "," << n << "," << k << "])";
        return os.str();
    }

    optional<vector<Shape>> MatmulObj::inferShape(const TensorVec &inputs)
    {

        // =================================== 作业 ===================================
        // TODO：返回经过 matmul 操作后的 shape
        // REF: https://github.com/onnx/onnx/blob/main/docs/Operators.md#gemm
        // =================================== 作业 ===================================
        auto output = inputs[0]->getDims();
        auto shapeA = inputs[0]->getDims();
        auto shapeB = inputs[1]->getDims();
        assert (shapeA.size() == shapeB.size());
        size_t pos = shapeA.size()-2;
        for(size_t i = 0; i < pos;++i){
            if(shapeA[i] == 1){
                shapeA[i] = shapeB[i];
            }else if (shapeB[i] == 1){
                shapeB[i] = shapeA[i];
            }
            output[i] = shapeA[i];
        }
        auto flag =(getTransA()<<1) + getTransB();
        switch (flag) {
        case 0x0:
            assert(shapeA[pos+1] == shapeB[pos+0]);
            output[pos] = shapeA[pos];
            output[pos+1] = shapeB[pos+1];
            break;
        case 0x1:
            assert(shapeA[pos+1] == shapeB[pos+1]);
            output[pos+0] = shapeA[pos+0];
            output[pos+1] = shapeB[pos+0];
            break;
        case 0x2:
            assert(shapeA[pos+0] == shapeB[pos+0]);
            output[pos+0] = shapeA[pos+1];
            output[pos+1] = shapeB[pos+1];
            break;
        case 0x3:
            assert(shapeA[pos+0] == shapeB[pos+1]);
            output[pos+0] = shapeA[pos+1];
            output[pos+1] = shapeB[pos+0];
            break;
        }

        return {{output}};
    }

} // namespace infini
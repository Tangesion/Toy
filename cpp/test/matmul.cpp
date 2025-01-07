#include "runtime/bufferManager.h"
#include "runtime/tensor.h"
#include "func/func.h"
#include "func/threadPool.h"
#include "kernel/cpu/matmul.h"
#include <fstream>
// #include <nlohmann/json.hpp>
#include <gtest/gtest.h>
#include "kernel/launch/matmul.h"

using namespace inference_frame::runtime;

// int main()
//{
//     // Tensor::randTensor({16, 1, 4}, dataType);
//
//     Tensor::SharedPtr tensor = inference_frame::func::randTensor({2, 2, 3, 4}, DataType::kFLOAT, MemoryType::kCPU);
//     std::cout << *tensor << std::endl;
//     // tensor->reshape({4, 3, 4});
//     inference_frame::func::reShape(tensor, {5, 3, 4});
//     std::cout << *tensor << std::endl;
//
//     // std::ifstream f("../config/support.json");
//     // nlohmann::json data = nlohmann::json::parse(f);
//     // std::cout << data.dump(4) << std::endl;
//     // std::cout << data["dataType"][0] << std::endl;
//     //  auto dims = Tensor::makeShape({16, 1, 4});
//     //  auto constexpr dataType = DataType::kFLOAT;
//     //  Tensor::SharedPtr tensor{BufferManager::cpu(dims, dataType)};
//     //  tensor->printShape();
// }
TEST(TensorTest, test)
{
    Tensor::SharedPtr tensor = inference_frame::func::randTensor({2, 2, 3, 4}, DataType::kFLOAT, MemoryType::kCPU);
    Tensor::DataType dataType = tensor->getDataType();
    std::cout << *tensor << std::endl;
    // tensor->reshape({4, 3, 4});
    inference_frame::func::reShape(tensor, {4, 3, 4});
    std::cout << *tensor << std::endl;
    auto data = inference_frame::func::getData(tensor);
    std::visit([&](auto &&ptr)
               {
        using T = std::decay_t<decltype(ptr)>;
        if constexpr (std::is_same_v<T, float*>) {
            std::cout << "Data type is float*" << std::endl;
            // 使用 ptr...
        } else if constexpr (std::is_same_v<T, int32_t*>) {
            std::cout << "Data type is int32_t*" << std::endl;
            // 使用 ptr...
        } else {
            throw std::runtime_error("Unsupported data type");
        } }, data);
}

TEST(MatmulTest, oneThreadTestTime)
{
    inference_frame::kernel::cpu::MatmulType matmulType = inference_frame::kernel::cpu::MatmulType::kMatmulOneThread;
    Tensor::SharedPtr inp = inference_frame::func::randTensor({1, 1024, 4096}, DataType::kFLOAT, MemoryType::kCPU);
    Tensor::SharedPtr weight = inference_frame::func::randTensor({4096, 4096}, DataType::kFLOAT, MemoryType::kCPU);
    // Tensor::SharedPtr bias = nullptr;
    Tensor::SharedPtr out = inference_frame::func::createTensor({1, 1024, 4096}, DataType::kFLOAT, MemoryType::kCPU);
    // inference_frame::kernel::cpu::matmulWeightLaunch(out, inp, weight, nullptr, matmulType);
    inference_frame::kernel::launch::matmulWeight(out, inp, weight, nullptr, matmulType);
}

TEST(MatmulTest, threadPoolTestTime)
{
    Tensor::SharedPtr inp = inference_frame::func::randTensor({1, 1024, 4096}, DataType::kFLOAT, MemoryType::kCPU);
    Tensor::SharedPtr weight = inference_frame::func::randTensor({4096, 4096}, DataType::kFLOAT, MemoryType::kCPU);
    // Tensor::SharedPtr bias = nullptr;
    Tensor::SharedPtr out = inference_frame::func::createTensor({1, 1024, 4096}, DataType::kFLOAT, MemoryType::kCPU);
    // inference_frame::kernel::cpu::matmulWeightLaunch(out, inp, weight, nullptr, inference_frame::kernel::cpu::MatmulType::kMatmulThreadPool);
    inference_frame::kernel::launch::matmulWeight(out, inp, weight, nullptr, inference_frame::kernel::cpu::MatmulType::kMatmulThreadPool);
}

TEST(MatmulTest, multiThreadTestTime)
{
    Tensor::SharedPtr inp = inference_frame::func::randTensor({1, 1024, 4096}, DataType::kFLOAT, MemoryType::kCPU);
    Tensor::SharedPtr weight = inference_frame::func::randTensor({4096, 4096}, DataType::kFLOAT, MemoryType::kCPU);
    // Tensor::SharedPtr bias = nullptr;
    Tensor::SharedPtr out = inference_frame::func::createTensor({1, 1024, 4096}, DataType::kFLOAT, MemoryType::kCPU);
    // inference_frame::kernel::cpu::matmulWeightLaunch(out, inp, weight, nullptr, inference_frame::kernel::cpu::MatmulType::KMatmulMultiThread);
    inference_frame::kernel::launch::matmulWeight(out, inp, weight, nullptr, inference_frame::kernel::cpu::MatmulType::KMatmulMultiThread);
}

// TEST(MatmulTest, equal)
//{
//     Tensor::SharedPtr inp = inference_frame::func::randTensor({1, 1024, 4096}, DataType::kFLOAT, MemoryType::kCPU);
//     Tensor::SharedPtr weight = inference_frame::func::randTensor({4096, 4096}, DataType::kFLOAT, MemoryType::kCPU);
//     // Tensor::SharedPtr bias = nullptr;
//     Tensor::SharedPtr out1 = inference_frame::func::createTensor({1, 1024, 4096}, DataType::kFLOAT, MemoryType::kCPU);
//     Tensor::SharedPtr out2 = inference_frame::func::createTensor({1, 1024, 4096}, DataType::kFLOAT, MemoryType::kCPU);
//     inference_frame::kernel::cpu::matmulWeightLaunch(out1, inp, weight, nullptr, inference_frame::kernel::cpu::MatmulType::kMatmulOneThread);
//     inference_frame::kernel::cpu::matmulWeightLaunch(out2, inp, weight, nullptr, inference_frame::kernel::cpu::MatmulType::KMatmulMultiThread);
//     auto *data1 = inference_frame::func::getData<Tensor::DataType::kFLOAT>(out1);
//     auto *data2 = inference_frame::func::getData<Tensor::DataType::kFLOAT>(out2);
//     for (int i = 0; i < out1->getSize(); i++)
//     {
//         ASSERT_EQ(data1[i], data2[i]);
//     }
// }
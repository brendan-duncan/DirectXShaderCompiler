// -T "cs_6_1" -E "Test" -wgsl "D:\\src\\DirectXShaderCompiler\\test_counter_buffers.hlsl"
RWStructuredBuffer<uint> ValueBuffer;
RWStructuredBuffer<uint> CountBuffer;

[numThreads(1, 1, 1)]
void Test(uint id : SV_DispatchThreadID) {
    ValueBuffer[id] = CountBuffer.IncrementCounter();
}

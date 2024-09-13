#include <benchmark/benchmark.h>
#include <vector>

static void BM_FtVector(benchmark::State& state) {
  // Benchmarkでは測らない前処理をここで書いておきます。
  std::vector<int> v;

  // このfor文の中にBenchmarkを測りたい処理を書きます。
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      v.push_back(i);
    }
  }
}
// Benchmarkを実行するには、このマクロを書きます。
// Range(1, 1 << 22)で、state.range(0)に段階的に渡す数を指定できます。
BENCHMARK(BM_FtVector)->Range(1, 1 << 16);

// main関数
BENCHMARK_MAIN();
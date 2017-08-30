#include <stdio.h>
#include <stdlib.h>

#include "BGenFile.h"

// #include <stdint.h>  // uint32_t
// #include <cassert>
// #include <string>
// #include <vector>

// #include <sqlite3.h>
// #include <zlib.h>
// #include "zstd/lib/zstd.h"

#define DEBUG
#undef DEBUG

void printVCFMeta() {
  // GP is between 0 and 1 in VCF v4.3, but phred-scaled value in VCF v4.2
  printf("##fileformat=VCFv4.3\n");
  if (false) {
    // it is not straightforward to know which to output without examining all
    // variant blocks
    printf(
        "##FORMAT=<ID=GT,Number=1,Type=String,Description=\"Genotype "
        "calls\">\n");
    printf(
        "##FORMAT=<ID=GP,Number=3,Type=Float,Description=\"Genotype call "
        "probabilities\">\n");
    printf(
        "##FORMAT=<ID=HP,Number=.,Type=Float,Description=\"Haplotype "
        "probabilities\">\n");
  }
}

void printVCFHeader(const std::vector<std::string>& sm) {
  printf("#CHROM\tPOS\tID\tREF\tALT\tQUAL\tFILTER\tINFO\tFORMAT");
  for (size_t i = 0; i != sm.size(); ++i) {
    printf("\t%s", sm[i].c_str());
  }
  printf("\n");
}

void printGeno(int i, const std::vector<int>& g) {
  printf("%d => ", i);
  for (int i = 0; i < g.size(); ++i) {
    printf(" %d", g[i]);
  }
  printf("\n");
}

void printVariant(const BGenVariant& var) {
  fputs(var.chrom.c_str(), stdout);
  fputs("\t", stdout);
  printf("%d", var.pos, stdout);
  fputs("\t", stdout);
  fputs(var.rsid.c_str(), stdout);
  // if (!var.varid.empty()) {
  //   fprintf(stdout, ",%s", var.varid.c_str());
  // }
  fputs("\t", stdout);
  fputs(var.alleles[0].c_str(), stdout);
  fputs("\t", stdout);
  for (size_t i = 1; i != var.alleles.size(); ++i) {
    if (i != 1) {
      fputs(",", stdout);
    }
    fputs(var.alleles[i].c_str(), stdout);
  }
  fputs("\t.\t.\t.", stdout);
  return;
  if (var.isPhased) {  // phased info
    fputs("\tGT:HP", stdout);
    for (int j = 0; j < var.N; ++j) {
      fputs("\t", stdout);
      var.printGT(j, stdout);
      fputs(":", stdout);
      var.printHP(j, stdout);
    }
  } else {  // genotypes
    fputs("\tGT:GP", stdout);
    for (int j = 0; j < var.N; ++j) {
      fputs("\t", stdout);
      var.printGT(j, stdout);
      fputs(":", stdout);
      var.printGP(j, stdout);
    }
  }
  fputs("\n", stdout);
}

int main(int argc, char* argv[]) {
  BGenIndex bgi(argv[1]);
  RangeList rl;
  rl.addRange("01", 2000, 5000);
  bgi.setRange(rl);
  int pos = 0;
  int size = 0;
  while (bgi.next(&pos, &size)) {
    printf("pos = %d size = %d\n", pos, size);
  }
  return 0;
}

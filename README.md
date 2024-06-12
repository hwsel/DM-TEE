# DM-TEE: Trusted Execution Environment for Disaggregated Memory

This project contains the source code fo the paper "DM-TEE: Trusted Execution Environment for Disaggregated Memory", which is published on GLSVLSI 2024. 
 
To build the client, we use SGX Dynamic Enclave Memory Manager (sgx-emm) in SGX. Please follow the SGX Linux Github page to download:
```
$ git clone https://github.com/intel/linux-sgx.git
```
Then please copy and paste the code in the client folder into ```linx_sgx/external/sgx-emm/``` and make :
```
$ cp -r ./client/ ../linux_sgx/external/sgx_emm/
$ make
```

Our disaggregated memory system is based on the Clio project, and we build a demo FPGA kernel structure on the Alveo U200 board. Our demo kernel is based on the [XUP Vitis Network Example](https://github.com/Xilinx/xup_vitis_network_example/). Please follow the command to compile the kernel:
```
$ git clone https://github.com/Xilinx/xup_vitis_network_example.git
$ cp -r ./SMNS/ ${path-to-vnx repo}/Basic_kernels/
$ make all TARGET=hw DESIGN=basic PLATFORM=${corresponding FPGA platform}
```

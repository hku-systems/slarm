## Building Enclave Leader Election

1. Install SGX SDK
    For HW, build and install
    For simulation, download source from https://github.com/intel/linux-sgx, build it and copy sdk/simulation/urtssim/linux/libsgx_urts_sim.so and ../sdk/simulation/uae_service_sim/linux/libsgx_uae_service_sim.so to /usr/lib/

2. build the project
    // for simulation only
    cmake ./; make clean; make; sudo make install
    // with hw build
    SGX_HW=1 cmake ./; make clean; make; sudo make install

3. change cgo LDFLAGS
    for simulation:
    #cgo LDFLAGS: -lpthread -L${SRCDIR}/lib -lnode_sgx_sim -ldl -L/opt/intel/sgxsdk/lib64/ -lsgx_uae_service_sim -lsgx_urts_sim
    for hw:
    #cgo LDFLAGS: -lpthread -L${SRCDIR}/lib -lnode_sgx_hw -ldl -L/opt/intel/sgxsdk/lib64/ -lsgx_urts


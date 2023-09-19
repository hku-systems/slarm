package enclave

/*
#include "lib/node.h"
#include <stdlib.h>
#cgo LDFLAGS: -lpthread -L${SRCDIR}/lib -ldl -L/home/yunpeng/code/sgx/sgxsdk/lib64 -lsgx_uae_service_sim -lsgx_urts_sim
*/
import "C"

// import (
// 	// "github.com/ethereum/go-ethereum/log"
// 	// "unsafe"
// 	"github.com/ethereum/go-ethereum/core/enclave/db"
// )

// for hardware mode
// #cgo LDFLAGS: -lpthread -L${SRCDIR}/lib -lnode_sgx_hw -ldl -L/opt/intel/sgxsdk/lib64/ -lsgx_urts
// for simulation
// #cgo LDFLAGS: -lpthread -L${SRCDIR}/lib -lnode_sgx_sim -ldl -L/opt/intel/sgxsdk/lib64/ -lsgx_uae_service_sim -lsgx_urts_sim
// for fake mode
// #cgo LDFLAGS: -lpthread -L${SRCDIR}/lib -lnode_sgx_fake
var timeoutms = 500

type Enclave struct {
	idx      string
	instance *C.Enclave_t
	Db       *QueryDB
}

func Test() {
	C.test()
}

// call during init
func NewEnclave() *Enclave {

	e := new(Enclave)
	e.instance = C.new_enclave()
	e.Db = NewQueryDB(e)

	// log.INFO("Successfully Create Enclave.")
	return e
}

func (e *Enclave) DestroyEnclave() {
	C.del_enclave(e.instance)
	// log.INFO("Successfully Killed Enclave.")
}

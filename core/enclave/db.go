package enclave

/*
#include "lib/node.h"
#include <stdlib.h>
#cgo LDFLAGS: -lpthread -L${SRCDIR}/lib  -ldl -L/home/yunpeng/code/sgx/sgxsdk/lib64 -lsgx_uae_service_sim -lsgx_urts_sim
*/
import "C"

import (
	"fmt"
	"unsafe"

	"github.com/ethereum/go-ethereum/core/types"
	"github.com/ethereum/go-ethereum/rlp"
)

type QueryDB struct {
	instance *C.Enclave_t
}

func NewQueryDB(e *Enclave) *QueryDB {
	C.test()
	qdb := new(QueryDB)
	qdb.instance = e.instance
	return qdb
}

func (m *QueryDB) Add(tx *types.Transaction) []uint64 {
	sliceBytes, err := rlp.EncodeToBytes(tx)
	if err != nil {
		fmt.Println("error")
		return nil
	}
	nonce := C.uint64_t(tx.GetUrgentNonce())
	urgent := C.int(0)
	if tx.Isurgent() {
		urgent = C.int(1)
	}
	ptr := (*C.char)(unsafe.Pointer(&sliceBytes[0]))
	ctxn := C.new_txn(ptr, C.int(len(sliceBytes)), urgent, nonce)
	cgaps := C.db_add(m.instance, ctxn)
	gapSize := int(cgaps.size)
	gaps := make([]uint64, gapSize)
	if len(gaps) != 0 {
		C.copy_array((*C.char)(unsafe.Pointer(&gaps[0])), cgaps.gaps, cgaps.size)
	}
	return gaps
}

func (queryDB *QueryDB) GetTx(urgent bool, nonce uint64) (*types.Transaction, bool) {
	cnonce := C.uint64_t(nonce)
	curgent := C.int(0)
	cok := C.int(0)
	if urgent {
		curgent = C.int(1)
	}
	ctxn := C.db_get(queryDB.instance, curgent, cnonce, &cok)
	if int(cok) == 0 {
		return nil, false
	}
	txn_bytes := C.GoBytes(unsafe.Pointer(ctxn.payload), ctxn.payload_size)
	tx := new(types.Transaction)
	if err := rlp.DecodeBytes(txn_bytes, tx); err != nil {
		// Return nil is not by mistake - some tests include sending transaction where gasLimit overflows uint64
		return nil, false
	}
	return tx, true
}

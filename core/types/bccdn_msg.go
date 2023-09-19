package types

import (
	"fmt"

	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/rlp"
)
type GapReqs struct {
	Reqs []*GapReq
}
type GapReq struct {
	Addr   common.Address
	Gaps   []uint64
	Urgent bool
    Data   []byte
}

type GapReqReply struct {
	Tx     Transaction
	Addr   common.Address
	Nonce  uint64
    Data   []byte
}


func (r GapReq) GetRlp () []byte {
	enc, _ := rlp.EncodeToBytes(r)
	return enc
}


func (r GapReq) String() string {
	return fmt.Sprintf("Addr:", r.Addr.String(), "Gaps:", r.Gaps, "Urgent:", r.Urgent)
}
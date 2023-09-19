package core

import (
	"fmt"
	"sync"

	"github.com/ethereum/go-ethereum/core/types"
)

type Gaps struct {
	sync.RWMutex
	UrgGaps 		map[uint64] bool
	NonUrgGaps      map[uint64] bool 
}

func NewGaps() *Gaps{
	return &Gaps{
		UrgGaps: 	nil,
		NonUrgGaps: nil,
	}
}

func (gaps *Gaps) Add(urgent bool, nonce uint64) {
	gaps.Lock()
	defer gaps.Unlock()
	fmt.Println("Qi Ji in Gaps.go/Add()", "urgent", urgent, "nonce", nonce)

	if urgent {
		if gaps.UrgGaps == nil {
			gaps.UrgGaps = make(map[uint64] bool)
		}
		gaps.UrgGaps[nonce] = true
		fmt.Println("Qi Ji in gaps.go/Add(), urgGaps", gaps.UrgGaps)
	} else {
		if gaps.NonUrgGaps == nil {
			gaps.NonUrgGaps = make(map[uint64] bool)
		}
		gaps.NonUrgGaps[nonce] = true
		fmt.Println("Qi Ji in gaps.go/Add(), nonUrgGaps", gaps.NonUrgGaps)
	}
}

func (gaps *Gaps) delete(tx *types.Transaction) {
	gaps.Lock()
	defer gaps.Unlock()

	fmt.Println("Qi Ji in gaps.go/delete()", "urgent", tx.Isurgent(), "nonce", tx.Nonce())

	nonce := tx.Nonce()
	urgent := tx.Isurgent()

	if urgent {
		if gaps.UrgGaps != nil {
			delete(gaps.UrgGaps, nonce)
			if len(gaps.UrgGaps) == 0 {
				gaps.UrgGaps = nil
			}
		}
		fmt.Println("Qi Ji in gaps.go/delete() after delete", "urgentGaps", gaps.UrgGaps)
	} else {
		if gaps.NonUrgGaps != nil {
			delete(gaps.NonUrgGaps, nonce)
			if len(gaps.NonUrgGaps) == 0 {
				gaps.NonUrgGaps = nil
			}
		}
		fmt.Println("Qi Ji in gaps.go/delete() after delete", "nonUrgentGaps", gaps.NonUrgGaps)
	}	
}

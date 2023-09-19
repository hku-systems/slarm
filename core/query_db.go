package core

import (
	"fmt"
	"sync"

	"github.com/ethereum/go-ethereum/core/types"
)

type DB struct {
	txs             map[uint64]*types.Transaction // Hash map storing the transaction data
	readyNonce		uint64
}

type QueryDB struct {
	sync.RWMutex
	urgentDB     *DB // Hash map storing the urgent transaction data
	nonUrgentDB  *DB // Hash map storing the non-urgent transaction data
}


func NewQueryDB() *QueryDB{
	return &QueryDB{
		urgentDB: newDB(),
		nonUrgentDB: newDB(),
	}
}

func newDB() *DB{
	return &DB{
		txs:   make(map[uint64]*types.Transaction),
		readyNonce: 0,
	}
}

func (m *QueryDB) Add(tx *types.Transaction) []uint64 {
	m.Lock()
	defer m.Unlock()
	var db *DB
	nonce := tx.Nonce()
	urgent := tx.Isurgent()

	if urgent {
		db = m.urgentDB
	} else {
		db = m.nonUrgentDB
	}

	if db.exist(nonce) {
		fmt.Println("Qi Ji in query_db.go/Add(), tx already exists, drop.", "urgent", tx.Isurgent(), "nonce", tx.Nonce())
		return nil
	}

	fmt.Println("Qi Ji in query_db.go/Add(), insert", "urgent", tx.Isurgent(), "nonce", tx.Nonce())

	db.txs[nonce] = tx
	db.incrementNonce()
	
	return db.detectGaps(urgent, nonce)
}

func (db *DB) exist(nonce uint64) bool {
	_, ok := db.txs[nonce]
	return ok
}


func (db *DB) incrementNonce() uint64 {
	i := db.readyNonce + 1
	for {
		_, ok := db.txs[i]
		if ok {
			i++
		} else {
			break
		}
	}
	db.readyNonce = i - 1
	fmt.Println("Qi Ji in query_db.go/incrementNonce() end", "readynonce", db.readyNonce)
	return db.readyNonce
}


func (db *DB) detectGaps(urgent bool, nonce uint64) []uint64 {
	fmt.Println("Qi Ji in query_db.go/requestGaps() 1", "start", db.readyNonce, "end", nonce)
	var gaps []uint64
	for i := db.readyNonce + 1; i < nonce; i++ {
		_, ok := db.txs[i]
		if !ok {
			gaps = append(gaps, i)
			fmt.Println("Qi Ji in query_db.go/requestGaps()", "gap", i)
		}
	}
	return gaps
}

func (queryDB *QueryDB) GetTx(urgent bool, nonce uint64) (*types.Transaction, bool) {
	queryDB.RLock()
	defer queryDB.RUnlock()

	var db *DB 
	if urgent {
		db =queryDB.urgentDB 
	} else {
		db =queryDB.nonUrgentDB 
	}	
	tx, ok := db.txs[nonce]
	return tx, ok
}
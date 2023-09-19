package main

import (
	"encoding/csv"
	"fmt"
	"os"
)

func main() {
	recordFile, err := os.Open("../nodeCaps.csv")
	if err != nil {
		fmt.Println("An error encountered ::", err)
	}
	// 2. Initialize the reader
	reader := csv.NewReader(recordFile)
	// 3. Read all the records
	records, _ := reader.ReadAll()
	// 4. Iterate through the records as you wish
	fmt.Println(records)
}
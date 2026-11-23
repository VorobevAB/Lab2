package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"project/structures"
)

func task2(args []string) {
	file := ""
	query := ""

	for i := 1; i < len(args); i++ {
		if args[i] == "--file" && i+1 < len(args) {
			i++
			file = args[i]
		} else if args[i] == "--query" && i+1 < len(args) {
			i++
			query = args[i]
		}
	}

	var set structures.StringSet
	objects := make([]string, 0)

	if file != "" {
		input, err := os.Open(file)
		if err == nil {
			scanner := bufio.NewScanner(input)
			for scanner.Scan() {
				line := scanner.Text()
				if line == "" {
					continue
				}

				fields := strings.Fields(line)
				if len(fields) >= 2 &&
					(fields[0] == "SETADD" || fields[0] == "SETDEL" || fields[0] == "SET_AT") {
					line = fields[1]
				}

				if !set.Contains(line) {
					set.Add(line)
					objects = append(objects, line)
				}
			}
			input.Close()
		}
	}

	execute := func(line string) {
		fields := strings.Fields(line)
		if len(fields) < 2 {
			return
		}

		op, val := fields[0], fields[1]

		switch op {
		case "SETADD":
			if !set.Contains(val) {
				set.Add(val)
				objects = append(objects, val)
			}
		case "SETDEL":
			if set.Contains(val) {
				set.Del(val)
				next := make([]string, 0, len(objects))
				for _, s := range objects {
					if s != val {
						next = append(next, s)
					}
				}
				objects = next
			}
		case "SET_AT":
			if set.Contains(val) {
				fmt.Println("TRUE")
			} else {
				fmt.Println("FALSE")
			}
		}
	}

	save := func() {
		if file == "" {
			return
		}
		output, err := os.Create(file)
		if err != nil {
			return
		}
		writer := bufio.NewWriter(output)
		for _, s := range objects {
			fmt.Fprintln(writer, s)
		}
		writer.Flush()
		output.Close()
	}

	if query != "" {
		execute(query)
		fields := strings.Fields(query)
		if len(fields) > 0 && (fields[0] == "SETADD" || fields[0] == "SETDEL") {
			save()
		}
	} else {
		scanner := bufio.NewScanner(os.Stdin)
		for scanner.Scan() {
			line := scanner.Text()
			execute(line)
			fields := strings.Fields(line)
			if len(fields) > 0 && (fields[0] == "SETADD" || fields[0] == "SETDEL") {
				save()
			}
		}
	}

	set.Clear()
}

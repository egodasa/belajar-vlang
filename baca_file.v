import os

fn main() {
	// `read_file` returns an optional (`?string`), it must be checked
	text := os.read_file('hello.v') or { // hasil baca file disimpan didalam variabel text
		println('failed to read the file')
		return
	}
  println('Hasil baca dari file')
  println(text)
  println('setelah dikonversikan ke array')
	lines := text.split_into_lines() // hasil bacaannya dari baris diubah jadi array
	for line in lines {
			println(line)
	}
}

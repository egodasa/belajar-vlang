import http
import json

struct Res {
  message string
  documentation_url string
}

fn main() {
	resp := http.get('https://api.github.com/user/repos') or {
		println('failed to fetch data from the server')
		return
	}
	result := json.decode(Res, resp.text) or { // json.decode untuk decode
		eprintln('Failed to parse json')
		return
	}
	println(resp.text)
}

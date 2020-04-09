import http
// http request menggunakan vlang
fn main() {
	resp := http.get('https://vlang.io/utc_now') or {
		println('failed to fetch data from the server')
		return
	}
	result := resp.text
	println(result)
}

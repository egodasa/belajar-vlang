import json

struct User {
	name          string
	age           int
}

fn main() {
	s := '[{"name":"Frodo", "age":25}, {"name":"Bobby", "age":10}]'
	users := json.decode([]User, s) or { // json.decode untuk decode
		eprintln('Failed to parse json')
		return
	}
	for user in users {
		println('$user.name: $user.age')
	}
	//~ println(json.encode(users)) // untuk encode
}

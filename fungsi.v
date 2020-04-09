fn main() {
  println(tambah(2, 10))
  println(dua_nilai(1,2))
}

fn tambah(alas, tinggi int) int {
  return alas + tinggi
}

fn dua_nilai(nilai1, nilai2 int) (int, int) {
  return nilai2, nilai1
}

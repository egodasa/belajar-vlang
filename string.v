/*
Semua variabel pada V lang bersifat immutable alias tidak bisa diubah
nilainya.
Jika ingin sebuah variabel bisa diubah nilainya, 
tambahkan "mut" saat deklarasi variabel

*/
fn main() {
  mut nama_lengkap := ''
  nama_depan := 'Ego'
  nama_belakang := 'Dasa'
  nama_lengkap = nama_depan + ' ' + nama_belakang // menggabungkan string menggunakan +
  println(nama_lengkap)
  println(nama_lengkap.len) // mengambil panjang string
  println(nama_lengkap[1..3]) // mengambil panjang string
}

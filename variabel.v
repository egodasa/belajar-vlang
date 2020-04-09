/*
Semua variabel pada V lang bersifat immutable alias tidak bisa diubah
nilainya.
Jika ingin sebuah variabel bisa diubah nilainya, 
tambahkan "mut" saat deklarasi variabel

*/
fn main() {
  nama := 'Madam'
  println(nama)
  
  //~ nama = 'madam baru' // kode ini tidak jalan, karena variabel nama imutable.
  //~ println(nama) 
  
  mut kelas := 10 // dengan adanya "mut", maka variabel kelas bis diganti isinya
  println(kelas)
  
  kelas = 20 // karena variabel kelas menggunakan "mut" saat deklarasi awal variabel, maka kode ini jalan
  println(kelas)
  
  mut nama_lengkap := ''
  nama_depan := 'Ego'
  nama_belakang := 'Dasa'
  nama_lengkap = nama_depan + ' ' + nama_belakang // menggabungkan string menggunakan +
  println(nama_lengkap)
  
  tinggi := 1.74
  println(tinggi)
  
  status := false
  println(status)
}

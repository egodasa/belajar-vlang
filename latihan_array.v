struct Mahasiswa {
  nobp string
  nm_mahasiswa string
  kelas string
}
fn main() {
  daftar_siswa := [
    Mahasiswa{
      nobp: '14101152610565'
      nm_mahasiswa: 'Ego Dafma Dasa'
      kelas: 'SI-10'
    },
    Mahasiswa{
      nobp: '15101152610047'
      nm_mahasiswa: 'Afri Yulianti'
      kelas: 'SI-7'
    },
    Mahasiswa{
      nobp: '14101152610045'
      nm_mahasiswa: 'Aidil'
      kelas: 'SI-10'
    },
    
  ]
	for nomor, mhs in daftar_siswa {
		println('$nomor\. \t $mhs.nm_mahasiswa \t $mhs.kelas')
	}
}

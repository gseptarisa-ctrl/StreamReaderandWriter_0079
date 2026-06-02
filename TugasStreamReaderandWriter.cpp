#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

// Class TokoElektronik untuk Manajemen Etalase (Poin 1 & 2)
class TokoElektronik {
private:
    // Array berkapasitas 3 elemen untuk merepresentasikan barang di etalase
    string etalase[3];

public:
    // Constructor untuk mengisi data awal secara otomatis
    TokoElektronik() {
        etalase[0] = "Televisi LED 32 Inch";
        etalase[1] = "Kulkas Dua Pintu";
        etalase[2] = "Mesin Cuci Otomatis";
    }

    // Method untuk mengambil produk dengan exception handling kustom
    string ambilProduk(size_t nomorRak) {
        try {
            // Mengubah array primitif menjadi objek vector sementara untuk memanfaatkan fungsi .at()
            // atau bisa juga menggunakan std::array jika didukung, di sini kita gunakan pembungkus vector aman.
            vector<string> vEtalase(begin(etalase), end(etalase));
            return vEtalase.at(nomorRak);
        }
        catch (const out_of_range& e) {
            // Melempar kembali (throw) pesan error kustom sesuai spesifikasi permintaan
            throw runtime_error("Gagal Mengambil Barang : Rak nomor " + to_string(nomorRak) + " kosong atau tidak tersedia!");
        }
    }
};

// Fungsi-fungsi Helper untuk Operasi CRUD Gudang Berbasis File (Poin 3)
const string FILE_NAME = "gudang.txt";

// Menampilkan semua barang dari file
void readGudang() {
    ifstream file(FILE_NAME);
    string line;
    int index = 1;
    
    cout << "\n====================================\n";
    cout << "      DAFTAR BARANG DI GUDANG       \n";
    cout << "====================================\n";
    
    if (!file.is_open()) {
        cout << "[Gudang Kosong / File Belum Terbuat]\n";
        return;
    }

    bool isEmpty = true;
    while (getline(file, line)) {
        if (!line.empty()) {
            cout << index++ << ". " << line << "\n";
            isEmpty = false;
        }
    }
    
    if (isEmpty) {
        cout << "[Gudang Kosong]\n";
    }
    file.close();
    cout << "====================================\n";
}

// Menambah barang ke dalam file (Create)
void createBarang() {
    ofstream file(FILE_NAME, ios::app); // ios::app untuk append/menambah di akhir baris
    if (!file.is_open()) {
        cout << "Gagal membuka file gudang!\n";
        return;
    }
    
    cin.ignore();
    string namaBarang;
    cout << "Masukkan nama barang baru: ";
    getline(cin, namaBarang);
    
    if (!namaBarang.empty()) {
        file << namaBarang << "\n";
        cout << "Barang berhasil ditambahkan ke gudang!\n";
    }
    file.close();
}

// Memperbarui data barang tertentu di dalam file (Update)
void updateBarang() {
    readGudang();
    ifstream fileInput(FILE_NAME);
    if (!fileInput.is_open()) return;

    vector<string> daftarBarang;
    string line;
    while (getline(fileInput, line)) {
        if (!line.empty()) {
            daftarBarang.push_back(line);
        }
    }
    fileInput.close();

    if (daftarBarang.empty()) return;

    int nomor;
    cout << "Pilih nomor barang yang ingin diubah: ";
    cin >> nomor;

    if (nomor > 0 && nomor <= static_cast<int>(daftarBarang.size())) {
        cin.ignore();
        string namaBaru;
        cout << "Masukkan nama baru untuk '" << daftarBarang[nomor - 1] << "': ";
        getline(cin, namaBaru);
        
        daftarBarang[nomor - 1] = namaBaru;

        // Tulis ulang seluruh data ke file
        ofstream fileOutput(FILE_NAME);
        for (const auto& barang : daftarBarang) {
            fileOutput << barang << "\n";
        }
        fileOutput.close();
        cout << "Data barang berhasil diperbarui!\n";
    } else {
        cout << "Nomor barang tidak valid!\n";
    }
}

// Menghapus data barang dari dalam file (Delete)
void deleteBarang() {
    readGudang();
    ifstream fileInput(FILE_NAME);
    if (!fileInput.is_open()) return;

    vector<string> daftarBarang;
    string line;
    while (getline(fileInput, line)) {
        if (!line.empty()) {
            daftarBarang.push_back(line);
        }
    }
    fileInput.close();

    if (daftarBarang.empty()) return;

    int nomor;
    cout << "Pilih nomor barang yang ingin dihapus: ";
    cin >> nomor;

    if (nomor > 0 && nomor <= static_cast<int>(daftarBarang.size())) {
        cout << "Barang '" << daftarBarang[nomor - 1] << "' telah dihapus.\n";
        daftarBarang.erase(daftarBarang.begin() + (nomor - 1));

        // Tulis ulang sisa data ke file
        ofstream fileOutput(FILE_NAME);
        for (const auto& barang : daftarBarang) {
            fileOutput << barang << "\n";
        }
        fileOutput.close();
    } else {
        cout << "Nomor barang tidak valid!\n";
    }
}

// Simulasi Pengambilan Barang di Etalase (Poin 4 - Pengujian)
void jalankanSimulasiEtalase(TokoElektronik& toko) {
    cout << "\n=== SIMULASI PENGUJIAN EXCEPTION HANDLING ===\n";
    
    // Skenario 1: Pengambilan barang di rak indeks ke-1 (Harus Sukses)
    cout << "[Skenario 1] Mengambil barang di rak indeks ke-1...\n";
    try {
        string barang = toko.ambilProduk(1);
        cout << "Hasil: Berhasil mengambil -> " << barang << "\n";
    } 
    catch (const exception& e) {
        cout << "Hasil: " << e.what() << "\n";
    }

    cout << "\n---------------------------------------------\n";

    // Skenario 2: Pengambilan barang di rak indeks ke-5 (Harus Gagal)
    cout << "[Skenario 2] Mengambil barang di rak indeks ke-5...\n";
    try {
        string barang = toko.ambilProduk(5);
        cout << "Hasil: Berhasil mengambil -> " << barang << "\n";
    } 
    catch (const exception& e) {
        cout << "Hasil: " << e.what() << "\n";
    }
    cout << "=============================================\n";
}

// Fungsi Utama / Main (Poin 4)
int main() {
    TokoElektronik tokoGibran;
    int pilihanMenu;

    do {
        // Otomatis membaca dan menampilkan daftar barang gudang setiap menu utama terbuka
        readGudang();

        cout << "\n=== SISTEM MANAJEMEN TOKO GIBRAN JAYA ===\n";
        cout << "1. Tambah Barang Gudang (Create)\n";
        cout << "2. Ubah Barang Gudang (Update)\n";
        cout << "3. Hapus Barang Gudang (Delete)\n";
        cout << "4. Jalankan Simulasi Etalase (Cek Exception)\n";
        cout << "5. Keluar Aplikasi\n";
        cout << "Pilih menu (1-5): ";
        cin >> pilihanMenu;

        switch (pilihanMenu) {
            case 1:
                createBarang();
                break;
            case 2:
                updateBarang();
                break;
            case 3:
                deleteBarang();
                break;
            case 4:
                jalankanSimulasiEtalase(tokoGibran);
                break;
            case 5:
                cout << "Keluar dari sistem. Terima kasih!\n";
                break;
            default:
                cout << "Pilihan tidak valid! Silakan pilih kembali.\n";
                break;
        }
        
        if (pilihanMenu != 5) {
            cout << "\nTekan Enter untuk kembali ke menu utama...";
            cin.ignore();
            cin.get();
        }

    } while (pilihanMenu != 5);

    return 0;
}
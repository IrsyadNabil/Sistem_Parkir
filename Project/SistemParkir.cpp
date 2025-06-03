#include <iostream>
#include <ctime>
#include <iomanip>
#include <conio.h>
#include <fstream>

using namespace std;

const int maxKendaraan = 100;
const double tarifMotor = 3000;
const double tarifMobil = 5000;
const double tarifTambahan = 2000;

struct Kendaraan
{
    string nomorPlat;
    string jenis;
    time_t waktuMasuk;
    bool sedangParkir;
};

struct SistemParkir
{
    Kendaraan daftarKendaraan[maxKendaraan];
    int jumlahKendaraan;
};

double hitungTarifParkir(string jenis, double jam)
{
    double tarifAwal = (jenis == "Motor" || jenis == "motor") ? tarifMotor : tarifMobil;

    if (jam <= 1)
        return tarifAwal;

    return tarifTambahan + hitungTarifParkir(jenis, jam - 1);
}

void kendaraanMasuk(SistemParkir &s)
{
    char kembali_masuk;
    do
    {
        system("cls");
        if (s.jumlahKendaraan >= maxKendaraan)
        {
            cout << "Parkiran penuh!\n";
            return;
        }

        Kendaraan k;
        cout << "Masukkan nomor plat kendaraan: ";
        cin.ignore();
        getline(cin, k.nomorPlat);
        cout << "Masukkan jenis kendaraan (Mobil/Motor): ";
        cin >> k.jenis;

        if (k.jenis != "Mobil" && k.jenis != "mobil" && k.jenis != "Motor" && k.jenis != "motor")
        {
            cout << "Jenis kendaraan tidak valid!\n";
        }

        else
        {
            k.waktuMasuk = time(0);
            k.sedangParkir = true;

            s.daftarKendaraan[s.jumlahKendaraan] = k;
            s.jumlahKendaraan++;
            cout << "Kendaraan berhasil masuk.\n";
        }
        cout << "Apakah anda ingin memasukkan data kembali? y/n : ";
        cin >> kembali_masuk;
    } while (kembali_masuk == 'y' || kembali_masuk == 'Y');

    getch();
}

void kendaraanKeluar(SistemParkir &s)
{
    system("cls");
    char kembali_keluar;
    if (s.jumlahKendaraan == 0)
    {
        cout << "Tidak ada kendaraan yang sedang parkir.\n";
        getch();
        return;
    }

    do
    {
        string plat;
        cout << "Masukkan nomor plat kendaraan yang keluar: ";
        cin.ignore();
        getline(cin, plat);
        bool kendaraanDitemukan = false;

        for (int i = 0; i < s.jumlahKendaraan; i++)
        {
            if (s.daftarKendaraan[i].sedangParkir && s.daftarKendaraan[i].nomorPlat == plat)
            {
                kendaraanDitemukan = true;
                time_t waktuKeluar = time(0);
                double durasi = difftime(waktuKeluar, s.daftarKendaraan[i].waktuMasuk) / 3600.0;
                double biaya = hitungTarifParkir(s.daftarKendaraan[i].jenis, durasi);

                cout << fixed << setprecision(2);
                cout << "Durasi parkir: " << durasi << " jam\n";
                cout << "Total biaya: Rp " << biaya << endl;

                s.daftarKendaraan[i].sedangParkir = false;

                for (int j = i; j < s.jumlahKendaraan - 1; j++)
                {
                    s.daftarKendaraan[j] = s.daftarKendaraan[j + 1];
                }
                s.jumlahKendaraan--;

                cout << "Kendaraan telah keluar.\n";
                getch();
                break;
            }
        }

        if (!kendaraanDitemukan)
        {
            cout << "Kendaraan tidak ditemukan!\n";
        }

        cout << "Apakah ada kendaraan yang ingin keluar lagi? (y/n): ";
        cin >> kembali_keluar;

    } while (kembali_keluar == 'y' || kembali_keluar == 'Y');

    return;
}

void tampilkanKendaraanParkir(SistemParkir &s)
{
    system("cls");
    if (s.jumlahKendaraan == 0)
    {
        cout << "Tidak ada kendaraan yang sedang parkir.\n";
        getch();
        return;
    }
    int sort;
    cout << "Pilih metode pengurutan:\n";
    cout << "1. Tanpa pengurutan\n";
    cout << "2. Ascending (berdasarkan nomor plat)\n";
    cout << "3. Descending (berdasarkan nomor plat)\n";
    cout << "Pilihan: ";
    cin >> sort;
    cin.ignore();
    system("cls");
    Kendaraan tempArray[maxKendaraan];
    int count = s.jumlahKendaraan;
    for (int i = 0; i < count; i++)
    {
        tempArray[i] = s.daftarKendaraan[i];
    }
    if (sort == 2)
    {
        for (int i = 0; i < count - 1; i++)
        {
            for (int j = 0; j < count - i - 1; j++)
            {
                if (tempArray[j].nomorPlat > tempArray[j + 1].nomorPlat)
                {
                    swap(tempArray[j], tempArray[j + 1]);
                }
            }
        }
    }
    else if (sort == 3)
    {
        for (int i = 0; i < count - 1; i++)
        {
            for (int j = 0; j < count - i - 1; j++)
            {
                if (tempArray[j].nomorPlat < tempArray[j + 1].nomorPlat)
                {
                    swap(tempArray[j], tempArray[j + 1]);
                }
            }
        }
    }
    else if (sort != 1)
    {
        cout << "Pilihan tidak valid, menampilkan tanpa pengurutan.\n";
    }

    cout << "\nDaftar Kendaraan yang Sedang Parkir:\n";
    cout << "-----------------------------------\n";
    cout << left << setw(15) << "No Plat" << setw(10) << "Jenis" << setw(20) << "Waktu Masuk" << endl;
    cout << "-----------------------------------\n";
    for (int i = 0; i < count; i++)
    {
        cout << left << setw(15) << tempArray[i].nomorPlat;
        cout << setw(10) << tempArray[i].jenis;
        cout << put_time(localtime(&tempArray[i].waktuMasuk), "%H:%M:%S") << endl;
    }
    getch();
}

void cariKendaraan(SistemParkir &s)
{
    system("cls");
    bool found;
    string plat;
    int a = 0;

    if (s.jumlahKendaraan == 0)
    {
        cout << "Tidak ada kendaraan yang sedang parkir.\n";
        getch();
        return;
    }
    cout << "Masukkan nomor plat kendaraan yang dicari: ";
    cin.ignore();
    getline(cin, plat);
    found = false;

    while (a < s.jumlahKendaraan && !found)
    {

        if (s.daftarKendaraan[a].sedangParkir && s.daftarKendaraan[a].nomorPlat == plat)
        {
            cout << "Kendaraan ditemukan!\n";
            cout << "Nomor Plat: " << s.daftarKendaraan[a].nomorPlat << "\n";
            cout << "Jenis: " << s.daftarKendaraan[a].jenis << "\n";
            cout << "Waktu Masuk: " << put_time(localtime(&s.daftarKendaraan[a].waktuMasuk), "%H:%M:%S") << "\n";
            getch();
            return;
        }
        a++;
    }

    cout << "Kendaraan tidak ditemukan!\n";
    getch();
}

void simpanDataKeFile(SistemParkir &s)
{
    ofstream outFile("data_parkir.txt");
    if (!outFile)
    {
        cout << "Gagal membuka file untuk penyimpanan.\n";
        return;
    }
    outFile << "Daftar Kendaraan Parkir:\n";
    outFile << "No Plat,Jenis,Waktu Masuk\n";
    outFile << "--------------------------\n";
    for (int i = 0; i < s.jumlahKendaraan; i++)
    {
        if (s.daftarKendaraan[i].sedangParkir)
        {
            time_t waktu = s.daftarKendaraan[i].waktuMasuk;
            tm *ltm = localtime(&waktu);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);

            outFile << s.daftarKendaraan[i].nomorPlat << ","
                    << s.daftarKendaraan[i].jenis << ","
                    << buffer << "\n";
        }
    }
    outFile.close();
    cout << "Data kendaraan parkir berhasil disimpan ke dalam file data_parkir.txt" << endl;
}

void bacaDataDariFile(SistemParkir &s)
{
    ifstream inFile("data_parkir.txt");
    if (!inFile)
    {
        cout << "File tidak ditemukan atau gagal dibuka.\n";
        getch();
        return;
    }
    s.jumlahKendaraan = 0;
    string header;
    getline(inFile, header);
    getline(inFile, header);
    getline(inFile, header);
    while (inFile.good() && s.jumlahKendaraan < maxKendaraan)
    {
        Kendaraan k;
        string line;
        getline(inFile, line);

        if (line.empty())
            continue;
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        if (pos1 == string::npos || pos2 == string::npos)
            continue;
        k.nomorPlat = line.substr(0, pos1);
        k.jenis = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string waktuStr = line.substr(pos2 + 1);
        tm waktuTm = {};
        istringstream ss(waktuStr);
        ss >> get_time(&waktuTm, "%Y-%m-%d %H:%M:%S");
        k.waktuMasuk = mktime(&waktuTm);
        k.sedangParkir = true;
        s.daftarKendaraan[s.jumlahKendaraan] = k;
        s.jumlahKendaraan++;
    }

    inFile.close();
    cout << "Data berhasil dimuat! Jumlah kendaraan: " << s.jumlahKendaraan << endl;
    getch();
}

int main()
{
    SistemParkir s;
    s.jumlahKendaraan = 0;
    int pilihan;

    while (true)
    {
        system("cls");
        cout << "=== SISTEM PARKIR ===\n";
        cout << "1. Kendaraan Masuk\n";
        cout << "2. Kendaraan Keluar\n";
        cout << "3. Lihat Kendaraan yang Parkir\n";
        cout << "4. Cari Kendaraan\n";
        cout << "5. Simpan Data ke File\n";
        cout << "6. Muat Data dari File\n";
        cout << "7. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            kendaraanMasuk(s);
            break;
        case 2:
            kendaraanKeluar(s);
            break;
        case 3:
            tampilkanKendaraanParkir(s);
            break;
        case 4:
            cariKendaraan(s);
            break;
        case 5:
            simpanDataKeFile(s);
            break;
        case 6:
            bacaDataDariFile(s);
            break;
        case 7:
            cout << "Terimakasih telah berkunjung\n";
            return 0;
        default:
            cout << "Pilihan tidak valid!\n";
        }
    }
}

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Lidar + Height Map Fusion Sistemi
- Lidar verisini txt'den okur
- Height map ile fusion yapar
- Yüksek çelişkiler varsa lidar verisine ağırlık verirçıktı: fused_heightmap.txt
"""

import numpy as np
import os
from lidar_sistem import AyAraciLidarSistemi

def lidar_input_oku(dosya_yolu="lidar_input.txt"):
    """
    Lidar input txt dosyasını okur
    Format:
    x y z
    0.5 0.5 25.3
    1.0 0.8 26.1
    ...
    """
    try:
        veri = np.loadtxt(dosya_yolu, skiprows=1)  # Header atlat
        if veri.ndim == 1:  # Tek satır ise
            veri = veri.reshape(1, -1)
        return veri.tolist()
    except FileNotFoundError:
        print(f"❌ {dosya_yolu} bulunamadı!")
        return []
    except Exception as e:
        print(f"❌ Hata: {e}")
        return []

def fusion_islemini_calistir(
    heightmap_dosyasi="input_heights.txt",
    lidar_dosyasi="lidar_input.txt",
    harita_boyutu=1845,
    cozunurluk=1.0,
    robot_x=0.0,
    robot_y=0.0,
    robot_yaw=0.0,
    celiski_esigi=0.15
):
    """
    Lidar + Height map fusion işlemini çalıştırır
    
    Parametreler:
    - heightmap_dosyasi: NASA height map dosyası
    - lidar_dosyasi: Lidar input dosyası (x y z)
    - harita_boyutu: Harita boyutu (1845x3849 için uyarla)
    - cozunurluk: Pixelperçev (1.0 = 1 metre)
    - robot_x, robot_y, robot_yaw: Robot konumu
    - celiski_esigi: Lidar-NASA çelişki eşiği
    """
    
    print("="*60)
    print("🛰️  LIDAR + HEIGHT MAP FUSION SISTEMI")
    print("="*60)
    
    # 1. Lidar sistemini başlat
    print(f"\n1️⃣  Lidar sistemi başlatılıyor...")
    print(f"   - Height map: {heightmap_dosyasi}")
    print(f"   - Harita boyutu: {harita_boyutu}")
    print(f"   - Çelişki eşiği: {celiski_esigi}")
    
    lidar_sistema = AyAraciLidarSistemi(
        dosya_yolu=heightmap_dosyasi,
        harita_boyutu=harita_boyutu,
        cozunurluk=cozunurluk
    )
    lidar_sistema.CELISKI_ESIGI = celiski_esigi
    
    # 2. Lidar verilerini oku
    print(f"\n2️⃣  Lidar verileri okunuyor...")
    lidar_verisi = lidar_input_oku(lidar_dosyasi)
    
    if not lidar_verisi:
        print("   ❌ Lidar verisi okunamadı!")
        return False
    
    print(f"   ✓ {len(lidar_verisi)} lidar noktası okundu")
    
    # 3. Fusion işlemini çalıştır
    print(f"\n3️⃣  Fusion işlemi yapılıyor...")
    print(f"   - Robot konumu: ({robot_x}, {robot_y}, {robot_yaw:.2f}°)")
    
    harita_degisti, guncel_harita = lidar_sistema.haritayi_isleme_ve_guncelle(
        lidar_verisi,
        [robot_x, robot_y, robot_yaw]
    )
    
    if harita_degisti:
        print(f"   ✓ Harita güncellendi (lidar verileri kullanıldı)")
    else:
        print(f"   ℹ  Harita güncellenmedi (çelişki eşiğine ulaşamadı)")
    
    # 4. Çıktı dosyasına kaydet
    print(f"\n4️⃣  Fused height map kaydediliyor...")
    output_dosya = "fused_heightmap.txt"
    
    try:
        np.savetxt(output_dosya, guncel_harita, fmt='%g')
        print(f"   ✓ {output_dosya} kaydedildi")
        print(f"   Boyut: {guncel_harita.shape}")
        print(f"   Min yükseklik: {np.min(guncel_harita):.2f}")
        print(f"   Max yükseklik: {np.max(guncel_harita):.2f}")
        print(f"   Ortalama: {np.mean(guncel_harita):.2f}")
    except Exception as e:
        print(f"   ❌ Kaydetme hatası: {e}")
        return False
    
    # 5. İstatistikleri göster
    print(f"\n5️⃣  İstatistikler:")
    print(f"   - Toplam piksel: {guncel_harita.size}")
    print(f"   - Güncellenen: {np.count_nonzero(np.diff(lidar_sistema.height_map.flatten()))} piksel")
    print(f"   - Standart sapma: {np.std(guncel_harita):.2f}")
    
    print("\n" + "="*60)
    print("✅ Fusion işlemi tamamlandı!")
    print("="*60 + "\n")
    
    return True

if __name__ == "__main__":
    # Varsayılan ayarlar
    harita_boyutu = 1845      # C++ programıyla uyumlu
    cozunurluk = 1.0          # 1 metre = 1 piksel
    celiski_esigi = 2.0       # 2 metre fark varsa lidar kullan
    
    # Fusion işlemini çalıştır
    fusion_islemini_calistir(
        heightmap_dosyasi="input_heights.txt",
        lidar_dosyasi="lidar_input.txt",
        harita_boyutu=harita_boyutu,
        cozunurluk=cozunurluk,
        robot_x=0.0,           # Robot başlangıç X
        robot_y=0.0,           # Robot başlangıç Y
        robot_yaw=0.0,         # Robot başlangıç yönü (radian)
        celiski_esigi=celiski_esigi
    )
    
    print("💡 Sonraki adım: heightmap_to_slopemap.cpp programını çalıştır")
    print("   Input: fused_heightmap.txt")
    print("   Output: slopemap.txt")

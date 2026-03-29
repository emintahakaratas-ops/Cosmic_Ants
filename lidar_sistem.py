import numpy as np
import os

class AyAraciLidarSistemi:
    def __init__(self, dosya_yolu="input_heights.txt", harita_boyutu=500, cozunurluk=0.1):
        """
        dosya_yolu: NASA verilerinin olduğu .txt dosyası
        harita_boyutu: Arkadaşından gelecek (Örn: 500)
        cozunurluk: Arkadaşından gelecek (Örn: 0.1)
        """
        self.map_size = harita_boyutu
        self.res = cozunurluk
        
        self.height_map = self.dosyadan_harita_yukle(dosya_yolu)
        
        self.ROBOT_YARICAP = 0.4    
        self.CELISKI_ESIGI = 0.15    
        
    def dosyadan_harita_yukle(self, dosya_yolu):
        """NASA'dan alınan heightmap.txt dosyasını okur."""
        if os.path.exists(dosya_yolu):
            try:
                yuklenen_veri = np.loadtxt(dosya_yolu)
                if yuklenen_veri.shape != (self.map_size, self.map_size):
                    print(f"Uyarı: Dosya boyutu {yuklenen_veri.shape} beklenenle uyuşmuyor!")
                return yuklenen_veri
            except Exception as e:
                print(f"Hata: Dosya okunurken sorun çıktı: {e}")
                return np.zeros((self.map_size, self.map_size))
        else:
            print("Uyarı: heightmap.txt bulunamadı, boş harita oluşturuluyor.")
            return np.zeros((self.map_size, self.map_size))

    def metre_to_index(self, x_m, y_m):
        """Metre koordinatını dizi indeksine çevirir (Sol Alt 0,0)."""
        ix = int(x_m / self.res)
        iy = int(y_m / self.res)
        return ix, iy

    def haritayi_isleme_ve_guncelle(self, ham_lidar_xyz, robot_pos):
        """
        ham_lidar_xyz: [[x, y, z], [x, y, z]...] -> Lidar'dan gelen direkt x,y,z verisi
        robot_pos: [gx, gy, yaw] -> Robotun dünya koordinatı ve yönü
        """
        rx, ry, ryaw = robot_pos
        harita_degisti = False
        
        for nokta in ham_lidar_xyz:
            lx, ly, lz = nokta
            
            # --- ADIM 1: GÖVDE FİLTRESİ ---
            dist = np.sqrt(lx**2 + ly**2)
            if dist < self.ROBOT_YARICAP:
                continue
            
            gx = rx + (lx * np.cos(ryaw) - ly * np.sin(ryaw))
            gy = ry + (lx * np.sin(ryaw) + ly * np.cos(ryaw))
            
            ix, iy = self.metre_to_index(gx, gy)
            
            if 0 <= ix < self.map_size and 0 <= iy < self.map_size:
                nasa_z = self.height_map[ix, iy]
                lidar_z = lz 
                
                if abs(lidar_z - nasa_z) > self.CELISKI_ESIGI:
                    self.height_map[ix, iy] = lidar_z
                    harita_degisti = True
                    
        return harita_degisti, self.height_map

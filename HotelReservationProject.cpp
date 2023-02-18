#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ODA_SAYISI 100

typedef struct{
	char *musteriAdi;
	char *musteriSoyadi;
}musteriBilgileri;

typedef struct{
	int gun;
	int ay;
	int yil;
}tarihBilgileri;

typedef struct{
	int kisiSayisi;
	int odaDurumu;//doluysa1,Bossa0
	int odaNumarasi;//1-100Arasinda
	double fiyatBilgisi;
	musteriBilgileri musteri;
	tarihBilgileri girisTarihi;
	tarihBilgileri cikisTarihi;
}odaBilgileri;

typedef enum{
	musteri=0,personel=1
}yetkiSeviyeleri;

int gunFarkiHesapla(tarihBilgileri girisTarihi,tarihBilgileri cikisTarihi){
	if(girisTarihi.gun>cikisTarihi.gun){
		cikisTarihi.ay-=1;
		cikisTarihi.gun+=30;
	}
	if(girisTarihi.ay>cikisTarihi.ay){
		cikisTarihi.yil-=1;
		cikisTarihi.ay+=12;
	}
	tarihBilgileri tarihFarki;
	tarihFarki.gun=cikisTarihi.gun-girisTarihi.gun;
	tarihFarki.ay=cikisTarihi.ay-girisTarihi.ay;
	tarihFarki.yil=cikisTarihi.yil-girisTarihi.yil;

	int gunFarki=tarihFarki.yil*365+tarihFarki.ay*30+tarihFarki.gun;
	return gunFarki;
}

double fiyatHesaplama(odaBilgileri *odalar,int odaNumarasi){
	int gunFarki=gunFarkiHesapla(odalar[odaNumarasi-1].girisTarihi,odalar[odaNumarasi-1].cikisTarihi);
	printf("Kalacaginiz Gun Sayisi:%d\n",gunFarki);
	return gunFarki*150*odalar[odaNumarasi-1].kisiSayisi;
}

void rezervasyonTemizle(odaBilgileri*odalar,int odaNumarasi){
	odalar[odaNumarasi-1].odaDurumu=0;//odayiBosalttik
	free(odalar[odaNumarasi-1].musteri.musteriAdi);
	free(odalar[odaNumarasi-1].musteri.musteriSoyadi);
	printf("%d numarali odanin cikis/iptal islemi tamamlanmistir.\n",odaNumarasi);
}

void yeniMusteriEkleme(odaBilgileri *odalar){
	
	int sorgulanacakOda;
	char musteriAdi[20],musteriSoyadi[20],ch;
	printf("Lutfen kayit olmak istediginiz odayi giriniz:");
	scanf("%d",&sorgulanacakOda);
	if(odalar[sorgulanacakOda-1].odaDurumu!=0){
		printf("Maalesef kayit yaptirmak istediginiz oda %d:%d:%d tarihine kadar doludur.",odalar[sorgulanacakOda-1].cikisTarihi.gun,odalar[sorgulanacakOda-1].cikisTarihi.ay,odalar[sorgulanacakOda-1].cikisTarihi.yil);
		return;	
	}
	
	odalar[sorgulanacakOda-1].odaNumarasi=sorgulanacakOda; //odaNumarasiAtandi
	
	printf("Lutfen ad ve soyad bilgilerini giriniz:");
	scanf("%s%s",&musteriAdi,&musteriSoyadi);
	odalar[sorgulanacakOda-1].musteri.musteriAdi=(char*)malloc(sizeof(char)*strlen(musteriAdi)+1);
	odalar[sorgulanacakOda-1].musteri.musteriSoyadi=(char*)malloc(sizeof(char)*strlen(musteriSoyadi)+1);
	strcpy(odalar[sorgulanacakOda-1].musteri.musteriAdi,musteriAdi);
	strcpy(odalar[sorgulanacakOda-1].musteri.musteriSoyadi,musteriSoyadi);
	printf("Lutfen kac kisi olacaginizi giriniz:");
	scanf("%d",&odalar[sorgulanacakOda-1].kisiSayisi);
	printf("Lutfen giris tarihi bilgisini (gun:ay:yil) seklinde giriniz");
	scanf("%d:%d:%d",&odalar[sorgulanacakOda-1].girisTarihi.gun,&odalar[sorgulanacakOda-1].girisTarihi.ay,&odalar[sorgulanacakOda-1].girisTarihi.yil);
	printf("Lutfen cikis tarihi bilgisini (gun:ay:yil) seklinde giriniz");
	scanf("%d:%d:%d",&odalar[sorgulanacakOda-1].cikisTarihi.gun,&odalar[sorgulanacakOda-1].cikisTarihi.ay,&odalar[sorgulanacakOda-1].cikisTarihi.yil);
	
	odalar[sorgulanacakOda-1].fiyatBilgisi=fiyatHesaplama(odalar,sorgulanacakOda);
	printf("Odemeniz gereken total tutar:%.2lf\n",odalar[sorgulanacakOda-1].fiyatBilgisi);
	printf("Onayliyor musunuz? (E-H):");
	fflush(stdin);
	ch=getchar();
	if(ch=='H'){
		rezervasyonTemizle(odalar,sorgulanacakOda);
		return;
	}
	odalar[sorgulanacakOda-1].odaDurumu=1; //odamizDolu
}

int main(){
	int belirlenenSifre=123456,kullanicininGirdigiSifre;
	yetkiSeviyeleri kullaniciYetkisi;
	char ch;
	
	odaBilgileri *odaDizisi;//tumBilgilerBuradaTutulacak
	odaDizisi=(odaBilgileri*)calloc(ODA_SAYISI,sizeof(odaBilgileri));//dinamikSekildeOlusturduk
	printf("\t\t\t Otel Rezervasyon Sistemine Hosgeldiniz..\n");
	printf("Lutfen burada calisiyorsaniz sifrenizi giriniz..");
	scanf("%d",&kullanicininGirdigiSifre);
	if(belirlenenSifre==kullanicininGirdigiSifre){//kullaniciOtelinBirCalisani,YetkisiPersonel
		kullaniciYetkisi=personel;
	}
	else{
		kullaniciYetkisi=musteri;
	}
	do{
		printf("1.) Yeni musteri girisi yapmak istiyorsaniz 'E'\n2.) Oda bilgisi sorgulamak istiyorsaniz 'S'\n3.) Dolu oda bilgilerini goruntulemek istiyorsaniz'G'\n4.) Fiyat sorgusu yapmak istiyorsaniz 'F'\n");
		printf("5.) Musteri cikisi gerceklestirmek istiyorsaniz 'C'\n6.) Sistemden cikis yapmak istiyorsaniz 'Q' tuslayiniz..");
		fflush(stdin);
		ch=getchar();
		switch(ch){
			case 'E':
				if(kullaniciYetkisi==personel){
					yeniMusteriEkleme(odaDizisi);
				}
				else{
					printf("Bu alan sadece calisanlar tarafindan kullanilabilir");
				}
				break;
			case 'S':
				break;
			case 'G':
				break;	
			case 'C':
				break;	
			case 'F':
				break;
			case 'Q':	
				break;
			default:
				break;
		}	
	}while(ch!='Q');
}  

//Ecem Büþra ERGÜN 170204069
#include <stdio.h>
#include <ctype.h>
#define TRUE 1
#define FALSE 0

const char Oyuncu = 'O';     // oyuncunun disk sembolü
const char Bilgisayar = 'X';   // bilgisayarýn disk sembolü
const char Blank = '.';      // oyun tahtasýndaki noktalar
const int MAX_SIZE = 20;            // oyun tahtasýnýn alabileceði max deðer

int size = 0;           // kullanýlan oyun tahtasý
int fullSquares = 0;    // toplam hareket sayýsý
int noMoreMoves = 0;	// legal hareket kontrolü
int whosTurnIs = TRUE;	// oynama sýrýsý: TRUE=oyuncu, FALSE=bilgisayar

// geçersiz giris yapýldýðýnda sistem arabelleðini temizlemek için
void clearInputBuffer(){ char junk; while ((junk = getchar()) != '\n' && junk != EOF);}
//int getSize(void);
void newBoard(char b[][MAX_SIZE]);
void placeCenter(char b[][MAX_SIZE]);
void printBoard(char b[][MAX_SIZE]);
char getOpponent(char user);
int isOutsideSquare(int satir, int col); // col - sutun
int isBlankSquare(char square);
int isLegalMove(char b[][MAX_SIZE], int satir, int col, int satir_inc, int col_inc, char user);
int getAllLegalMoves(char b[][MAX_SIZE], int m[][MAX_SIZE], char user);
void getInputMove(char b[][MAX_SIZE], int m[][MAX_SIZE]);
int isCaptured(char b[][MAX_SIZE], int* x, int* y, int satir, int col, int satir_inc, int col_inc, char user);
void makeMove(char b[][MAX_SIZE], int satir, int col, char user);
int bestScore(char b[][MAX_SIZE], int m[][MAX_SIZE], char user);
void calculateBestComputerMove(char b[][MAX_SIZE], int m[][MAX_SIZE], int* satirB, int* colB);
void computersMove(char b[][MAX_SIZE], int m[][MAX_SIZE]);
void printFinalScore(char b[][MAX_SIZE]);
void gameLoop(char b[][MAX_SIZE], int m[][MAX_SIZE]);
void gameLoopMultiplayer(char b[][MAX_SIZE], int m[][MAX_SIZE]);

int main()
{
	char board[MAX_SIZE][MAX_SIZE];
	int moves[MAX_SIZE][MAX_SIZE];
	int oyunTuru = 0, wasSuccessful;

	printf("<<< HADI OYUN BASLASIN! >>>\n");
	size = 8;			// kullanýlan oyun tahtasý boyutu

	newBoard(board);			// yeni oyun tahtasý yaratýr
	placeCenter(board);			

	printf("Oyun turu secin: 1 - Bilgisayar, 2 - Multiplayer: ");
	wasSuccessful = scanf("%d", &oyunTuru);
	
	while(wasSuccessful != 1 || (wasSuccessful == 1 && (oyunTuru != 1 && oyunTuru != 2) )){
		clearInputBuffer();
		printf("Girdi 1 veya 2 olmali!!!\n");
		printf("Oyun turu secin: 1 - Bilgisayar, 2 - Multiplayer: ");
		wasSuccessful = scanf("%d", &oyunTuru);
	}

	if(oyunTuru == 1){
		printf("Bilgisayara karsi oynuyorsun\n");
		gameLoop(board, moves);		// Game loop Bilgisayar
	}
	else if(oyunTuru == 2){
		printf("Multiplayer oynuyorsun\n");
		gameLoopMultiplayer(board, moves); // Game loop Multiplayer
	}
		
	printBoard(board);			// final board
	printFinalScore(board);		// final score

	return 0;
}

// boyut için konsoldan girdi almak için
//int getSize=;
	/*int integer, wasSuccessful;
	
	char junk;

    printf("Oyun tahtasi en az 4, en fazla %d olabilmektedir.\n");
	while(TRUE)
	{

		printf("Lutfen tahta boyutunu seciniz: ");
		wasSuccessful = scanf("%d", &integer);

		// giriþ tam sayý deðilse
		if(wasSuccessful != 1)
		{
			printf("Girilen sayi tamsayi olmalidir. Tekrar deneyiniz.!\n");
			clearInputBuffer();
		}
		// Tam sayý aralýk dýþýndaysa
		else if(integer < 4 || integer > MAX_SIZE) 
		{
			printf("Tahta boyutu 4 ile %d arasinda olmalidir. Tekrar deneyiniz!\n", MAX_SIZE);
		}
		// tam sayý çift deðilse
		else if(integer%2 == 1)
		{
			printf("Girilen sayi cift sayi olmalidir. Tekrar deneyiniz!\n");
		}
		else{
			//InputBuffer() temizleme;
			return integer;
		}
	}
}*/

// sembolleri tahtaya yazdýrmak için fonksiyon
void newBoard(char b[][MAX_SIZE]){
	int i, j;

	// her kullanýcý için boþ sembolü
    for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            b[i][j] = Blank;
}

void placeCenter(char b[][MAX_SIZE]){
	b[size/2 - 1][size/2 - 1] = Bilgisayar;
	b[size/2][size/2] = Bilgisayar;
	b[size/2 - 1][size/2] = Oyuncu;
	b[size/2][size/2 - 1] = Oyuncu;

	fullSquares = 4;    // merkezi 4 diskin kare miktarini arttýrýr 
}

// ekrana oyun tahtasýný sýralý bir þekilde yazdýran fonksiyon
void printBoard(char b[][MAX_SIZE]){
	int satir = 0, column = 0;	// satýr ve col içeriði
	char columnLetter = 'a';	// col harflendirmesi
	int satirsay = 0;			// satýr numaralandýrmasý

	// col harflendirmesini yazdýrma iþlemi
	printf("\n  ");
	for(column = 0; column < size; column++)
		printf("%2c", columnLetter++);
	printf("\n");

	// satýr numarasýný yazdýrma ve iki boyutlu dizinin deðerleri
	for(satir = 0; satir < size; satir++)
	{
		printf("%2d", ++satirsay);
		for(column = 0; column < size; column++)
			printf("%2c", b[satir][column]);
		printf("\n");
	}
	printf("\n");
}

// karþý tarafý/rakibi  belirlemek için fonksiyon
char getOpponent(char user){
	if(user == Oyuncu)
		return Bilgisayar;
	else if (user == Bilgisayar)
		return Oyuncu;
	else
		printf("Rakip tanimlama hatasi!\n");
}

// kare tahta dýþýndaysa diye kontrol eden fonksiyon
int isOutsideSquare(int satir, int col){
	if(satir < 0 || satir >= size || col < 0 || col >= size)
		return TRUE;
	else
		return FALSE;
}

// kare boþsa diye kontrol eden fonksiyon
int isBlankSquare(char square){
	if(square == Blank)
		return TRUE;
	else
		return FALSE;
}

// hareketýn legal olup olmadýðýný kontrol eden fonksiyon
int isLegalMove(char b[][MAX_SIZE], int satir, int col, int satir_inc, int col_inc, char user){
	int x = 0, y = 0;				// satýr - col 
	char opponent = getOpponent(user);

	
	if(isOutsideSquare(satir+satir_inc, col+col_inc) || (satir_inc==0 && col_inc==0))
		return FALSE;

	if(b[satir + satir_inc][col + col_inc] == opponent)//hangi tarafa doðru gittiðini kontrol ediyor
	{
		x = satir + satir_inc;
		y = col + col_inc;			

		// kullanýcýnýn karesi ile ayný yöndekinleri kontrol etme
		while(TRUE)
		{
			x += satir_inc;		
			y += col_inc;		
			
			
			if(isOutsideSquare(x, y) || isBlankSquare(b[x][y]))
				return FALSE;

			
			if(b[x][y] == user)
				return TRUE;
		}
	}
	else
		return FALSE;
}

// Tahtadaki bütün olasý hareketler için fonksiyon
int getAllLegalMoves(char b[][MAX_SIZE], int m[][MAX_SIZE], char user){
	int i, j, satir, col, satir_inc, col_inc; // döngü sayaçlarý
	int totalLegalMoves = 0;	// legal hareket sayýsý

	// dizideki legal hareketleri silme kontrolü
	for(i = 0; i < size; i++)
		for(j = 0; j < size; j++)
			m[i][j] = FALSE;

	// bütün legal hareketleri bulup harelet dizisinde iþaretleme.
	for(satir = 0; satir < size; satir++)
		for(col = 0; col < size; col++)
		{
			// kare boþ deðilse bir sonraki kareye atlama kontrol etme
			if(!isBlankSquare(b[satir][col]))
				continue;

			// rakip disk için boþ kare kontrol etme
			for(satir_inc = -1; satir_inc <= 1; satir_inc++)
				for(col_inc = -1; col_inc <= 1; col_inc++)
					if(isLegalMove(b, satir, col, satir_inc, col_inc, user))
					{
						m[satir][col] = TRUE;
						totalLegalMoves++;
					}
		}

	return totalLegalMoves;
}

// kullanýcýdan girdi alýp kontrol etmek için fonksiyon
void getInputMove(char b[][MAX_SIZE], int m[][MAX_SIZE], char user){
	char columnLetter = 0;
	int columnNumber = 0, satirsay = 0, wasSuccessful;

	while(TRUE)
	{
		printf("Hareketinizi yapiniz... [satir + kol]: ");
		wasSuccessful = scanf("%d%c", &satirsay, &columnLetter);

		if(wasSuccessful != 2)
		{
			printf("Yanlýþ veri girisi. Tekrar dene.\n");
			clearInputBuffer();
			continue;
		}

		columnNumber = tolower(columnLetter) - 97;
		satirsay--;

		if(m[satirsay][columnNumber] && satirsay>=0 && satirsay<size 
									 && columnNumber>=0 && columnNumber<size)
		{
			makeMove(b, satirsay, columnNumber, user);
			fullSquares++;
			break;
		}
		else{
			printf("yaptigin hareket illegal, tekrar dene.\n");
			clearInputBuffer();
		}
	}
}
//tas alýnýp alýnmadýðý
// rakip diskin yakalanýp yakalanmadýðýný kontrol etmek için fonksiyon
int isCaptured(char b[][MAX_SIZE], int *x, int *y, int satir, int col, int satir_inc, int col_inc, char user){
	char opponent = getOpponent(user);

	// kare dýþýný kontrol etme
	if(isOutsideSquare(satir+satir_inc, col+col_inc) || (satir_inc==0 && col_inc==0))
		return FALSE;

	if(b[satir + satir_inc][col + col_inc] == opponent)
	{
		*x = satir + satir_inc;
		*y = col + col_inc;

		while(TRUE)
		{
			*x += satir_inc;
			*y += col_inc;

		
			if(isOutsideSquare(*x, *y) || isBlankSquare(b[*x][*y]))
				return FALSE;

		
			if(b[*x][*y] == user)
				return TRUE;
		}
	}
	else
		return FALSE;
}

// tahtadaki hareketi yapan fonksiyon 
void makeMove(char b[][MAX_SIZE], int satir, int col, char user){
	char opponent = getOpponent(user);
	int x = 0, y = 0;
	int satir_inc, col_inc; // döngü sayaç

	b[satir][col] = user;

	// rakip disklerin kontrolü ve mümkünse disk yakalama
	for(satir_inc = -1; satir_inc <= 1; satir_inc++)
		for(col_inc = -1; col_inc <= 1; col_inc++)
			if(isCaptured(b, &x, &y, satir, col, satir_inc, col_inc, user))
				while(b[x-=satir_inc][y-=col_inc] == opponent)
					b[x][y] = user;
}

// bilgisayar icin en mantýklý hareket ile en iyi puaný bulan fonksiyon
int bestScore(char b[][MAX_SIZE], int m[MAX_SIZE][MAX_SIZE], char user){
	char tempBoard[MAX_SIZE][MAX_SIZE];
	char opponent = getOpponent(user);
	int scoreBest = 0, score = 0;

	int satir, col, i, j, satirS, colS; // döngü sayaçlarý

	newBoard(tempBoard);

	for(satir = 0; satir < size; satir++)
		for(col = 0; col < size; col++)
		{
			
			if(m[satir][col] == FALSE)
				continue;

			// geçici diziye atama
			for(i = 0; i < size; i++)
				for(j = 0; j < size; j++)
					tempBoard[i][j] = b[i][j];

			makeMove(tempBoard, satir, col, user);

			// tahtadaki bütün kareler,puan kontrolü
			for(satirS = 0; satirS < size; satirS++)
			{
				for(colS = 0; colS < size; colS++)
				{ 
					if(b[satirS][colS] == Bilgisayar)
						score++;
					else if(b[satirS][colS] == Oyuncu)
						score--;
				}
			}
			// en iyi puaný kaydetme
			if(score > scoreBest)
				scoreBest = score;
		}
	return scoreBest;
}

// bilgisayarýn en iyi hareketini hesaplayan fonksiyon
void calculateBestComputerMove(char b[][MAX_SIZE], int m[][MAX_SIZE], int* satirB, int* colB){
	char tempBoard[MAX_SIZE][MAX_SIZE];
	int tempMoves[MAX_SIZE][MAX_SIZE];
	int lowestScore = 0, highestScore = size*size;

	int i, j, satir, col; // döngü sayaçlarý

	newBoard(tempBoard);

	// geçici hamle dizisi
	for(i = 0; i < size; i++)
		for(j = 0; j < size; j++)
			tempMoves[i][j] = FALSE;

	// geçici tahtada hareket yapma
	for(satir = 0; satir < size; satir++)
		for(col = 0; col < size; col++)
		{
			if(m[satir][col] == FALSE)
				continue;

			// geçici tahtaya kopyalama iþlemi
			for(i = 0; i < size; i++)
				for(j = 0; j < size; j++)
					tempBoard[i][j] = b[i][j];

			// bilgisayarýn geçici tahtadaki hareketi
			makeMove(tempBoard, satir, col, Bilgisayar);

			// bilgisayarýn hareketinden sonra rakip icin legal hareketleri bulma
			getAllLegalMoves(tempBoard, tempMoves, Oyuncu);

			// rakip için en düþük en iyi puaný bulma 
			lowestScore = bestScore(tempBoard, tempMoves, Oyuncu);

			//bilgisayarýn hareketinden sonra rakip için legal hareketleri bulma
			if(lowestScore < highestScore)
			{
				highestScore = lowestScore;
				// en iyi hareket için satir ve col numarýsýný kaydetme
				*satirB = satir;
				*colB = col;
			}
		}
}

// bilgisayar en iyi hareketini yapmak için fonksiyon
void computersMove(char b[][MAX_SIZE], int m[][MAX_SIZE]){
	int satirBest = 0, colBest = 0;

	// en iyi hareketin satir col numarasi
	calculateBestComputerMove(b, m, &satirBest, &colBest);

	// bilgisayar için en iyi hareketin satýr kol numarasý
	makeMove(b, satirBest, colBest, Bilgisayar); 
}

// Bitiþ ve bitiþ skorunu yazan fonksiyon
void printFinalScore(char b[][MAX_SIZE]){
	int playerScore = 0, computerScore = 0, blank = 0;

	int satir, col; // döngü sayaç

	// oyun tahtasýndaki bütün kareleri kontrol etmek için
	for(satir = 0; satir < size; satir++)
		for(col = 0; col < size; col++)
		
			if(b[satir][col] == Oyuncu)
                playerScore++;
			
			else if (b[satir][col] == Bilgisayar)
				computerScore++;
			
			else
				blank++;

	printf("Game Over! ");

	if(computerScore > playerScore)
		printf("Bilgisayar Kazandi!\n");
	else if(playerScore > computerScore)
		printf("Oyuncu Kazandi!\n");
	else	// berabere kalma durumu
		printf("Oyun Berabere Bitti!\n");

	printf("Final Scores:\n");
	printf("Bilgisayar: %d, Oyuncu: %d\n", computerScore, playerScore);
}

// oyundaki döngü fonksiyonu
void gameLoop(char b[][MAX_SIZE], int m[][MAX_SIZE]){
	do
	{
		printBoard(b);

		switch(whosTurnIs)
		{
			case TRUE: // sýra oyuncuda
				if(getAllLegalMoves(b, m, Oyuncu))
				{
					// Geçerli bir giriþ olana kadar oyuncu giriþlerini oku
					clearInputBuffer();
					getInputMove(b, m, Oyuncu);
				}
				else //oyuncu için legal hareket yok
				{
					noMoreMoves++;
					if(noMoreMoves<2)
					{
						// geçici bellekte girilen deðerleri boþaltmak için
						clearInputBuffer();
						printf("Bilgisayar:Yapabilecegin hareket yok, benim sýram!\n");
					}
					else
						printf("Computer: Hic legal hareket kalmadý, oyun bitti!\n");
				}
				whosTurnIs = FALSE;
				break;

			case FALSE: // Bilgisayarda sýra
				if(getAllLegalMoves(b, m, Bilgisayar)) 
				{
					computersMove(b, m);
					fullSquares++;			// toplam hareket sayýsýný arttýrma
					noMoreMoves = 0;
				}
				else // Bilgisayarýn legal hareketinin kalmayýþý
				{
					noMoreMoves++;
					if(noMoreMoves<2) 
						printf("Computer: Yapabilecegim hareket yok, sira sende.\n");
					else
						printf("Computer: Hic legal hareket kalmadý, oyun bitti!\n");
				}
				whosTurnIs = TRUE;
				break;
		}
	}while(fullSquares < size*size && noMoreMoves<2);
}

void gameLoopMultiplayer(char b[][MAX_SIZE], int m[][MAX_SIZE])
{
	do
	{
		printBoard(b);

		switch(whosTurnIs)
		{
			case TRUE: // sýra 1. oyuncuda
				if(getAllLegalMoves(b, m, Oyuncu))
				{
					// Geçerli bir giriþ olana kadar 1. oyuncu giriþlerini oku
					printf("Sira 1. oyuncuda, sembol[%c]!\n", Oyuncu);
					clearInputBuffer();
					getInputMove(b, m, Oyuncu);
				}
				else //oyuncu için legal hareket yok
				{
					noMoreMoves++;
					if(noMoreMoves<2)
					{
						// geçici bellekte girilen deðerleri boþaltmak için
						clearInputBuffer();
						printf("1. Oyuncu:Yapabilecegim hareket yok, sira sende!\n");
					}
					else
						printf("Bilgilendirme: Hic legal hareket kalmadý, oyun bitti!\n");
				}
				whosTurnIs = FALSE;
				break;

			case FALSE: // sýra 2. oyuncuda
 				if(getAllLegalMoves(b, m, Bilgisayar)) 
				{
					// Geçerli bir giriþ olana kadar 2. oyuncu giriþlerini oku
					printf("Sira 2. oyuncuda, sembol[%c]!\n", Bilgisayar);
					clearInputBuffer();
					getInputMove(b, m, Bilgisayar);
				}
				else // Bilgisayarýn legal hareketinin kalmayýþý
				{
					noMoreMoves++;
					if(noMoreMoves<2) 
						printf("2. Oyuncu: Yapabilecegim hareket yok, sira sende.\n");
					else
						printf("Bilgilendirme: Hic legal hareket kalmadý, oyun bitti!\n");
				}
				whosTurnIs = TRUE;
				break;
		}
	}while(fullSquares < size*size && noMoreMoves<2);	
	
}

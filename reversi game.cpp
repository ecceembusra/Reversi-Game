//Ecem B��ra ERG�N 170204069
#include <stdio.h>
#include <ctype.h>
#define TRUE 1
#define FALSE 0

const char Oyuncu = 'O';     // oyuncunun disk sembol�
const char Bilgisayar = 'X';   // bilgisayar�n disk sembol�
const char Blank = '.';      // oyun tahtas�ndaki noktalar
const int MAX_SIZE = 20;            // oyun tahtas�n�n alabilece�i max de�er

int size = 0;           // kullan�lan oyun tahtas�
int fullSquares = 0;    // toplam hareket say�s�
int noMoreMoves = 0;	// legal hareket kontrol�
int whosTurnIs = TRUE;	// oynama s�r�s�: TRUE=oyuncu, FALSE=bilgisayar

// ge�ersiz giris yap�ld���nda sistem arabelle�ini temizlemek i�in
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
	size = 8;			// kullan�lan oyun tahtas� boyutu

	newBoard(board);			// yeni oyun tahtas� yarat�r
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

// boyut i�in konsoldan girdi almak i�in
//int getSize=;
	/*int integer, wasSuccessful;
	
	char junk;

    printf("Oyun tahtasi en az 4, en fazla %d olabilmektedir.\n");
	while(TRUE)
	{

		printf("Lutfen tahta boyutunu seciniz: ");
		wasSuccessful = scanf("%d", &integer);

		// giri� tam say� de�ilse
		if(wasSuccessful != 1)
		{
			printf("Girilen sayi tamsayi olmalidir. Tekrar deneyiniz.!\n");
			clearInputBuffer();
		}
		// Tam say� aral�k d���ndaysa
		else if(integer < 4 || integer > MAX_SIZE) 
		{
			printf("Tahta boyutu 4 ile %d arasinda olmalidir. Tekrar deneyiniz!\n", MAX_SIZE);
		}
		// tam say� �ift de�ilse
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

// sembolleri tahtaya yazd�rmak i�in fonksiyon
void newBoard(char b[][MAX_SIZE]){
	int i, j;

	// her kullan�c� i�in bo� sembol�
    for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            b[i][j] = Blank;
}

void placeCenter(char b[][MAX_SIZE]){
	b[size/2 - 1][size/2 - 1] = Bilgisayar;
	b[size/2][size/2] = Bilgisayar;
	b[size/2 - 1][size/2] = Oyuncu;
	b[size/2][size/2 - 1] = Oyuncu;

	fullSquares = 4;    // merkezi 4 diskin kare miktarini artt�r�r 
}

// ekrana oyun tahtas�n� s�ral� bir �ekilde yazd�ran fonksiyon
void printBoard(char b[][MAX_SIZE]){
	int satir = 0, column = 0;	// sat�r ve col i�eri�i
	char columnLetter = 'a';	// col harflendirmesi
	int satirsay = 0;			// sat�r numaraland�rmas�

	// col harflendirmesini yazd�rma i�lemi
	printf("\n  ");
	for(column = 0; column < size; column++)
		printf("%2c", columnLetter++);
	printf("\n");

	// sat�r numaras�n� yazd�rma ve iki boyutlu dizinin de�erleri
	for(satir = 0; satir < size; satir++)
	{
		printf("%2d", ++satirsay);
		for(column = 0; column < size; column++)
			printf("%2c", b[satir][column]);
		printf("\n");
	}
	printf("\n");
}

// kar�� taraf�/rakibi  belirlemek i�in fonksiyon
char getOpponent(char user){
	if(user == Oyuncu)
		return Bilgisayar;
	else if (user == Bilgisayar)
		return Oyuncu;
	else
		printf("Rakip tanimlama hatasi!\n");
}

// kare tahta d���ndaysa diye kontrol eden fonksiyon
int isOutsideSquare(int satir, int col){
	if(satir < 0 || satir >= size || col < 0 || col >= size)
		return TRUE;
	else
		return FALSE;
}

// kare bo�sa diye kontrol eden fonksiyon
int isBlankSquare(char square){
	if(square == Blank)
		return TRUE;
	else
		return FALSE;
}

// hareket�n legal olup olmad���n� kontrol eden fonksiyon
int isLegalMove(char b[][MAX_SIZE], int satir, int col, int satir_inc, int col_inc, char user){
	int x = 0, y = 0;				// sat�r - col 
	char opponent = getOpponent(user);

	
	if(isOutsideSquare(satir+satir_inc, col+col_inc) || (satir_inc==0 && col_inc==0))
		return FALSE;

	if(b[satir + satir_inc][col + col_inc] == opponent)//hangi tarafa do�ru gitti�ini kontrol ediyor
	{
		x = satir + satir_inc;
		y = col + col_inc;			

		// kullan�c�n�n karesi ile ayn� y�ndekinleri kontrol etme
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

// Tahtadaki b�t�n olas� hareketler i�in fonksiyon
int getAllLegalMoves(char b[][MAX_SIZE], int m[][MAX_SIZE], char user){
	int i, j, satir, col, satir_inc, col_inc; // d�ng� saya�lar�
	int totalLegalMoves = 0;	// legal hareket say�s�

	// dizideki legal hareketleri silme kontrol�
	for(i = 0; i < size; i++)
		for(j = 0; j < size; j++)
			m[i][j] = FALSE;

	// b�t�n legal hareketleri bulup harelet dizisinde i�aretleme.
	for(satir = 0; satir < size; satir++)
		for(col = 0; col < size; col++)
		{
			// kare bo� de�ilse bir sonraki kareye atlama kontrol etme
			if(!isBlankSquare(b[satir][col]))
				continue;

			// rakip disk i�in bo� kare kontrol etme
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

// kullan�c�dan girdi al�p kontrol etmek i�in fonksiyon
void getInputMove(char b[][MAX_SIZE], int m[][MAX_SIZE], char user){
	char columnLetter = 0;
	int columnNumber = 0, satirsay = 0, wasSuccessful;

	while(TRUE)
	{
		printf("Hareketinizi yapiniz... [satir + kol]: ");
		wasSuccessful = scanf("%d%c", &satirsay, &columnLetter);

		if(wasSuccessful != 2)
		{
			printf("Yanl�� veri girisi. Tekrar dene.\n");
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
//tas al�n�p al�nmad���
// rakip diskin yakalan�p yakalanmad���n� kontrol etmek i�in fonksiyon
int isCaptured(char b[][MAX_SIZE], int *x, int *y, int satir, int col, int satir_inc, int col_inc, char user){
	char opponent = getOpponent(user);

	// kare d���n� kontrol etme
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
	int satir_inc, col_inc; // d�ng� saya�

	b[satir][col] = user;

	// rakip disklerin kontrol� ve m�mk�nse disk yakalama
	for(satir_inc = -1; satir_inc <= 1; satir_inc++)
		for(col_inc = -1; col_inc <= 1; col_inc++)
			if(isCaptured(b, &x, &y, satir, col, satir_inc, col_inc, user))
				while(b[x-=satir_inc][y-=col_inc] == opponent)
					b[x][y] = user;
}

// bilgisayar icin en mant�kl� hareket ile en iyi puan� bulan fonksiyon
int bestScore(char b[][MAX_SIZE], int m[MAX_SIZE][MAX_SIZE], char user){
	char tempBoard[MAX_SIZE][MAX_SIZE];
	char opponent = getOpponent(user);
	int scoreBest = 0, score = 0;

	int satir, col, i, j, satirS, colS; // d�ng� saya�lar�

	newBoard(tempBoard);

	for(satir = 0; satir < size; satir++)
		for(col = 0; col < size; col++)
		{
			
			if(m[satir][col] == FALSE)
				continue;

			// ge�ici diziye atama
			for(i = 0; i < size; i++)
				for(j = 0; j < size; j++)
					tempBoard[i][j] = b[i][j];

			makeMove(tempBoard, satir, col, user);

			// tahtadaki b�t�n kareler,puan kontrol�
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
			// en iyi puan� kaydetme
			if(score > scoreBest)
				scoreBest = score;
		}
	return scoreBest;
}

// bilgisayar�n en iyi hareketini hesaplayan fonksiyon
void calculateBestComputerMove(char b[][MAX_SIZE], int m[][MAX_SIZE], int* satirB, int* colB){
	char tempBoard[MAX_SIZE][MAX_SIZE];
	int tempMoves[MAX_SIZE][MAX_SIZE];
	int lowestScore = 0, highestScore = size*size;

	int i, j, satir, col; // d�ng� saya�lar�

	newBoard(tempBoard);

	// ge�ici hamle dizisi
	for(i = 0; i < size; i++)
		for(j = 0; j < size; j++)
			tempMoves[i][j] = FALSE;

	// ge�ici tahtada hareket yapma
	for(satir = 0; satir < size; satir++)
		for(col = 0; col < size; col++)
		{
			if(m[satir][col] == FALSE)
				continue;

			// ge�ici tahtaya kopyalama i�lemi
			for(i = 0; i < size; i++)
				for(j = 0; j < size; j++)
					tempBoard[i][j] = b[i][j];

			// bilgisayar�n ge�ici tahtadaki hareketi
			makeMove(tempBoard, satir, col, Bilgisayar);

			// bilgisayar�n hareketinden sonra rakip icin legal hareketleri bulma
			getAllLegalMoves(tempBoard, tempMoves, Oyuncu);

			// rakip i�in en d���k en iyi puan� bulma 
			lowestScore = bestScore(tempBoard, tempMoves, Oyuncu);

			//bilgisayar�n hareketinden sonra rakip i�in legal hareketleri bulma
			if(lowestScore < highestScore)
			{
				highestScore = lowestScore;
				// en iyi hareket i�in satir ve col numar�s�n� kaydetme
				*satirB = satir;
				*colB = col;
			}
		}
}

// bilgisayar en iyi hareketini yapmak i�in fonksiyon
void computersMove(char b[][MAX_SIZE], int m[][MAX_SIZE]){
	int satirBest = 0, colBest = 0;

	// en iyi hareketin satir col numarasi
	calculateBestComputerMove(b, m, &satirBest, &colBest);

	// bilgisayar i�in en iyi hareketin sat�r kol numaras�
	makeMove(b, satirBest, colBest, Bilgisayar); 
}

// Biti� ve biti� skorunu yazan fonksiyon
void printFinalScore(char b[][MAX_SIZE]){
	int playerScore = 0, computerScore = 0, blank = 0;

	int satir, col; // d�ng� saya�

	// oyun tahtas�ndaki b�t�n kareleri kontrol etmek i�in
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

// oyundaki d�ng� fonksiyonu
void gameLoop(char b[][MAX_SIZE], int m[][MAX_SIZE]){
	do
	{
		printBoard(b);

		switch(whosTurnIs)
		{
			case TRUE: // s�ra oyuncuda
				if(getAllLegalMoves(b, m, Oyuncu))
				{
					// Ge�erli bir giri� olana kadar oyuncu giri�lerini oku
					clearInputBuffer();
					getInputMove(b, m, Oyuncu);
				}
				else //oyuncu i�in legal hareket yok
				{
					noMoreMoves++;
					if(noMoreMoves<2)
					{
						// ge�ici bellekte girilen de�erleri bo�altmak i�in
						clearInputBuffer();
						printf("Bilgisayar:Yapabilecegin hareket yok, benim s�ram!\n");
					}
					else
						printf("Computer: Hic legal hareket kalmad�, oyun bitti!\n");
				}
				whosTurnIs = FALSE;
				break;

			case FALSE: // Bilgisayarda s�ra
				if(getAllLegalMoves(b, m, Bilgisayar)) 
				{
					computersMove(b, m);
					fullSquares++;			// toplam hareket say�s�n� artt�rma
					noMoreMoves = 0;
				}
				else // Bilgisayar�n legal hareketinin kalmay���
				{
					noMoreMoves++;
					if(noMoreMoves<2) 
						printf("Computer: Yapabilecegim hareket yok, sira sende.\n");
					else
						printf("Computer: Hic legal hareket kalmad�, oyun bitti!\n");
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
			case TRUE: // s�ra 1. oyuncuda
				if(getAllLegalMoves(b, m, Oyuncu))
				{
					// Ge�erli bir giri� olana kadar 1. oyuncu giri�lerini oku
					printf("Sira 1. oyuncuda, sembol[%c]!\n", Oyuncu);
					clearInputBuffer();
					getInputMove(b, m, Oyuncu);
				}
				else //oyuncu i�in legal hareket yok
				{
					noMoreMoves++;
					if(noMoreMoves<2)
					{
						// ge�ici bellekte girilen de�erleri bo�altmak i�in
						clearInputBuffer();
						printf("1. Oyuncu:Yapabilecegim hareket yok, sira sende!\n");
					}
					else
						printf("Bilgilendirme: Hic legal hareket kalmad�, oyun bitti!\n");
				}
				whosTurnIs = FALSE;
				break;

			case FALSE: // s�ra 2. oyuncuda
 				if(getAllLegalMoves(b, m, Bilgisayar)) 
				{
					// Ge�erli bir giri� olana kadar 2. oyuncu giri�lerini oku
					printf("Sira 2. oyuncuda, sembol[%c]!\n", Bilgisayar);
					clearInputBuffer();
					getInputMove(b, m, Bilgisayar);
				}
				else // Bilgisayar�n legal hareketinin kalmay���
				{
					noMoreMoves++;
					if(noMoreMoves<2) 
						printf("2. Oyuncu: Yapabilecegim hareket yok, sira sende.\n");
					else
						printf("Bilgilendirme: Hic legal hareket kalmad�, oyun bitti!\n");
				}
				whosTurnIs = TRUE;
				break;
		}
	}while(fullSquares < size*size && noMoreMoves<2);	
	
}

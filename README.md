#### CMIS: Криптографические методы защиты информации #### 

--

##### VIGENEER.CPP: #####
  1.  программа работает со строчными латинскими буквами;  
  2.  шифрование происходит на основе двух формул:  

	-e: C[i] = (P[i] + K[i]) % 26 + 97;  
	-d: P[i] = (C[i] - K[i] + 26) % 26 + 97;    
      
      C - зашфрованное сообщение;  
      K -ключ;  
      P - оригинал сообщения.  
      
      \* прибавляем 97, чтобы получить ASCII-код символа. 
      
    
  3.  функция CopyString - копируем ключ в конец существующего до совпадения длин ключа и текста;  
     функция GetIndex - находим индекс очередного символа в алфавите.  

--
   
##### AES.CPP: #####
  1.  шифрование методом AES-128;
  2.  используются регламентированные в стандарте значения 4-4-10:
      4 столбца для матрицы состояний;
      4 * DWORD - длина ключа (128 бит);
      10 раундов шифрования.
  3.  --help - вызов небольшого справочника;
  4.  для того, чтобы длина сообщения была кратна 16 и при этом криптоустойчива,
     добавляется в конец сообщения символ '1', а остальное заполняется нулями;
  5.  программа состоит из 4 трансформация надо матрицей состояния - statе:
     shift_rows(0 or 1) - циклический сдвиг влево/вправо i-ой строки на i элементов;
		 sub_bytes(0 or 1) - замена каждого байта из state на соответствующий из sbox/inv_sbox;
		 key_expansion() - формирование раундовых ключей - блок длиной 4*(10+1);
		 add_rkey(0 or 1, number of round) - побитовый xor элементов из state и word;
		 mix_columns(0 or 1) - перемножение столбцов из state и многочлена  3x^3 + x^2 + x + 2
		                       по модулю x^4 + 1 в поле Галуа GF(256) (подсмотрено на habrahabr.ru).
   
--

##### STG.CPP: #####
   1.  стеганография с изображениями формата BMP (32 бита/пиксель);
   2.  --help - вызов небольшого справочника;
   3.  все использованные структуры содержатся в wingdi.h;
   4.  структура файла: 
      - файл начинается с заголовка, состоящего из двух структур: BITMAPFILEHEADER и BITMAPINFOHEADER, - итого 54 байта.
      - в некоторых источниках говорится, что и в 32-битных изображениях может далее идти палитра размером 4*256 байт 
        (хотя описание цвета идет абсолютное - RGB), на этот случай для перестраховки все содержимое файла до смещения
        BITMAPFILEHEADER.OffBits записываем в h_buffer[54 + 1024];
      - следом по смещению BITMAPFILEHEADER.OffBits начинается массив пикселей, один пиксель описывается структурой
        RGBQUAD (4 байта): 3 байта (blue, green, red) и 1 резервный байт (reserved), - в описании говорится,
        что reserved должен быть обнулен,по сути же, байт нигде не используется, поэтому именно туда программа
        записывает сообщение;
      - размер файла должен быть кратен 4-м, поэтому в конце он дополняется нулями.

--

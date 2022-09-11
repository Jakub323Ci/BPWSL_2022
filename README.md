# BPWSL_2022

Repozytorium zawierające załączniki do pracy magisterskiej "Badanie pracy wątków w systemie wbudowanym z systemem operacyjnym Linux"

## Kroki potrzebne do pobrania, przygotowania i uruchomienia programów (sposób 1)

Na komputerze z systemem operacyjnym z jądrem Linux:
- Uruchomić terminal
- Przejść do wybranego folderu w którym ma znajdować się folder z repozytorium: `cd <wybrany_folder>`
- Skopiować i uruchomić poniższe polecenie: 
`git clone https://github.com/Jakub323Ci/BPWSL_2022`
- Wejść do folderu z repozytorium: 
`cd BPWSL_2022`
- Aby skompilować wybrany program należy użyć następującego polecenia: 
`gcc <wybrany_plik_z_kodem_zrodlowym.c> -o <nazwa_pliku_wykonywalnego> -lpthread`
na przykład: 
`gcc cond_var_2.c -o cond_var_2 -lpthread`
- Uruchomić program w następujący sposób: `./<nazwa_pliku_wykonywalnego>`, np. `./cond_var_2`

## Alternatywny sposób (sposób 2)

W przypadku istniejącego już IDE w systemie, lub z jakiegokolwiek innego powodu, można również skopiować same kody źródłowe programów w repozytorium i skompilować je w inny, wybrany przez siebie sposób. Należy jednak pamiętać o zlinkowaniu biblioteki pthreads (w przypadku gcc będzie to dodanie opcji -lpthread)

- Wejść w wybrany plik z kodem źródłowym w repozytorium
- Skopiować zawartość kodu do wybranego przez siebie edytora tekstu, IDE, etc.
- Skompilowanie i zbudowanie programu.
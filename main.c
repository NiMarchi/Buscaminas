#define SDL_MAIN_HANDLED

#include "juego.h"
#include "menu.h"
#include "variables.h"
#include "ventana.h"

// Función principal.
int main(int argc, char* argv[]) {
	// Si initialize_window está inicializado, game_is_running es igual a verdadero.
	gameRunning = initializeWindow();

	while (gameRunning) {
		setupMainMenu();

		while (mainMenuRunning) {
			processInput(); // Procesar las entradas del usuario mediante el mouse/teclado en el menú.
			update(); // Procesar estados de objetos del juego en el menú.
			render(); // Procesar la representación de objetos en el menú.
		}

		while (selectMenuRunning) {
			processInput(); // Procesar las entradas del usuario mediante el mouse/teclado en el menú.
			update(); // Procesar estados de objetos del juego en el menú.
			render(); // Procesar la representación de objetos en el menú.
		}

		if(stageRunning) {
			setupStage(h, m); // Inicializa los parámetros del campo minado.
		}

		while (stageRunning) {
			processInput(); // Procesar las entradas del usuario mediante el mouse y el teclado en el escenario.
			update(); // Procesar los estados de los objetos del juego en el escenario.
			render(); // Procesar la representación de objetos en etapa.
			waitInterval(); // Cuando finaliza un juego, espera 3 segundos y detiene la música antes de volver al menú principal.
		}

		while (historyMenuRunning) {
			processInput();
			update();
			render();
		}
	}

	destroyWindow(); // Cierra la ventana y finaliza el proceso.
	return 0;
}
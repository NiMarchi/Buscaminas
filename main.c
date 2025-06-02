#define SDL_MAIN_HANDLED

#include "juego.h"
#include "menu.h"
#include "variables.h"
#include "ventana.h"

// Función principal.
int main(int argc, char* argv[]) {
	// Si initialize_window está inicializado, game_is_running es igual a verdadero.
	game_is_running = initialize_window();
	main_menu_is_running = game_is_running;

	while (game_is_running) {
		setup_main_menu();

		while (main_menu_is_running) {
			process_input(); // Procesar las entradas del usuario mediante el mouse/teclado en el menú.
			update(); // Procesar estados de objetos del juego en el menú.
			render(); // Procesar la representación de objetos en el menú.
		}

		while (select_menu_is_running) {
			process_input(); // Procesar las entradas del usuario mediante el mouse/teclado en el menú.
			update(); // Procesar estados de objetos del juego en el menú.
			render(); // Procesar la representación de objetos en el menú.
		}

		if(stage_is_running) {
			setup_stage(w, h, m); // Inicializa los parámetros del campo minado.
		}

		while (stage_is_running) {
			process_input(); // Procesar las entradas del usuario mediante el mouse y el teclado en el escenario.
			update(); // Procesar los estados de los objetos del juego en el escenario.
			render(); // Procesar la representación de objetos en etapa.
			wait_interval(); // Cuando finaliza un juego, espera 3 segundos y detiene la música antes de volver al menú principal.
		}
	}

	destroy_window(); // Cierra la ventana y finaliza el proceso.
	return 0;
}
#include <nds.h>
#include <stdio.h>
#include <math.h>

// --- Definiciones del Juego ---
#define screenWidth 256
#define screenHeight 192
#define mapWidth 24
#define mapHeight 24

// --- Mapa del Mundo ---
int worldMap[mapWidth][mapHeight] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,2,2,2,2,0,0,3,0,3,0,3,0,0,4,4,4,4,4,0,0,1},
    {1,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,4,0,0,0,4,0,0,1},
    {1,0,0,2,0,0,2,0,0,3,0,0,0,3,0,0,4,0,0,0,4,0,0,1},
    {1,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,4,0,0,0,4,0,0,1},
    {1,0,0,2,2,2,2,0,0,3,0,3,0,3,0,0,4,4,4,0,4,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Sistema de vida
int playerHP = 100;
int maxHP = 100;

// FPS Counter
int fpsCounter = 0;
int displayFPS = 60;
int lastSecond = 0;

// Pantalla de éxito
bool gameWon = false;
bool showVictory = false;

// Efecto visual - Fog of War
float fogDistance = 12.0f;

// Pantalla de bienvenida
bool showMenu = true;

int main(void) {
    // --- Estado del Jugador ---
    double posX = 22, posY = 12;      
    double dirX = -1, dirY = 0;       
    double planeX = 0, planeY = 0.66; 

    // --- Temporizadores y velocidad ---
    double moveSpeed = 0.1;
    double rotSpeed = 0.1;


    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);
    
    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankC(VRAM_C_SUB_BG);

    int bg = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0, 1);
    consoleDemoInit();

    //  PALETA CON SOMBREADO SUTIL
    BG_PALETTE[0] = RGB15(4, 5, 9);      // Cielo azul oscuro
    BG_PALETTE[1] = RGB15(24, 8, 8);     // Rojo frontal
    BG_PALETTE[2] = RGB15(8, 24, 8);     // Verde frontal
    BG_PALETTE[3] = RGB15(8, 15, 24);    // Azul frontal
    BG_PALETTE[4] = RGB15(28, 28, 28);   // Blanco frontal
    BG_PALETTE[5] = RGB15(28, 20, 4);    // Oro frontal
    BG_PALETTE[6] = RGB15(12, 12, 12);   // Gris medio (horizonte/fog)
    BG_PALETTE[7] = RGB15(16, 16, 16);   // Suelo gris claro
    
    // Colores laterales
    BG_PALETTE[8] = RGB15(21, 7, 7);     // Rojo lateral (casi igual al frontal)
    BG_PALETTE[9] = RGB15(7, 21, 7);     // Verde lateral (casi igual al frontal)
    BG_PALETTE[10] = RGB15(7, 13, 21);   // Azul lateral (casi igual al frontal)
    BG_PALETTE[11] = RGB15(25, 25, 25);  // Blanco lateral (casi igual al frontal)
    BG_PALETTE[12] = RGB15(25, 18, 3);   // Oro lateral (casi igual al frontal)
    
    BG_PALETTE[13] = RGB15(28, 15, 7);   // Naranja marco
    BG_PALETTE[14] = RGB15(31, 31, 8);   // Amarillo jugador
    BG_PALETTE[15] = RGB15(2, 2, 3);     // Negro/Fog muy oscuro


    u8* tileData = (u8*)bgGetGfxPtr(bg);
    for(int i = 0; i < 16; i++) {
        for(int p = 0; p < 64; p++) {
            tileData[i * 64 + p] = i;
        }
    }

    u16* mapBuffer = (u16*)bgGetMapPtr(bg);
    static int frameCount = 0;

    //  PANTALLA DE BIENVENIDA 
    while(showMenu) {
        scanKeys();
        int keys = keysDown();
        
        for(int i = 0; i < 32*24; i++) mapBuffer[i] = 0;
        
        for(int x = 0; x < 32; x++) {
            mapBuffer[0 * 32 + x] = 13;
            mapBuffer[23 * 32 + x] = 13;
        }
        for(int y = 0; y < 24; y++) {
            mapBuffer[y * 32 + 0] = 13;
            mapBuffer[y * 32 + 31] = 13;
        }
        
        iprintf("\x1b[2J");
        iprintf("\x1b[3;6H=== DS RAYCASTER v2.0 ===");
        iprintf("\x1b[6;4HCONTROLES:");
        iprintf("\x1b[7;5HFlechas: Mover/Girar");
        iprintf("\x1b[8;5HA/B: Strafe Izq/Der");
        iprintf("\x1b[9;5HL/R: Lento/Rapido");
        iprintf("\x1b[11;4HOBJETIVO:");
        iprintf("\x1b[12;5HEncuentra la sala dorada");
        iprintf("\x1b[14;6HPresiona START");
        
        if(keys & KEY_START) {
            showMenu = false;
            iprintf("\x1b[2J");
        }
        
        swiWaitForVBlank();
    }

    iprintf("DS RAYCASTER v2.0 - ENHANCED\n");
    iprintf("--- Encuentra la sala dorada ---\n");

    //  BUCLE PRINCIPAL 
    while(1) {
        
        if(showVictory) {
            scanKeys();
            int keys = keysDown();
            
            // Pantalla de victoria con colores dorados parpadeantes
            int goldColor = (frameCount % 20 < 10) ? 5 : 12;
            for(int i = 0; i < 32*24; i++) {
                mapBuffer[i] = goldColor;
            }
            
            // Muestra el texto de victoria
            iprintf("\x1b[2J");
            iprintf("\x1b[8;6HVICTORIA");
            iprintf("\x1b[9;4HEncontraste la sala dorada!");
            iprintf("\x1b[11;6HTiempo: %d segundos", frameCount/60);
            iprintf("\x1b[12;6HVida final: %d/%d", playerHP, maxHP);
            iprintf("\x1b[14;4HPresiona START para salir");
            
            if(keys & KEY_START) break;
            
            frameCount++;
            swiWaitForVBlank();
            continue; 
        }
        
        // Controles
        scanKeys();
        int keys = keysHeld();

        if (keys & KEY_UP) {
            int destValX = worldMap[(int)(posX + dirX * moveSpeed)][(int)posY];
            if(destValX == 0 || destValX == 5) posX += dirX * moveSpeed;
            
            int destValY = worldMap[(int)posX][(int)(posY + dirY * moveSpeed)];
            if(destValY == 0 || destValY == 5) posY += dirY * moveSpeed;
        }
        if (keys & KEY_DOWN) {
            int destValX = worldMap[(int)(posX - dirX * moveSpeed)][(int)posY];
            if(destValX == 0 || destValX == 5) posX -= dirX * moveSpeed;

            int destValY = worldMap[(int)posX][(int)(posY - dirY * moveSpeed)];
            if(destValY == 0 || destValY == 5) posY -= dirY * moveSpeed;
        }
        
        if (keys & KEY_LEFT) {
            double oldDirX = dirX;
            dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
            dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
            planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        }
        if (keys & KEY_RIGHT) {
            double oldDirX = dirX;
            dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
            dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
            planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(rotSpeed);
        }

        if (keys & KEY_A) { // Strafe Izquierda
            int destValX = worldMap[(int)(posX + planeX * moveSpeed)][(int)posY];
            if(destValX == 0 || destValX == 5) posX += planeX * moveSpeed;

            int destValY = worldMap[(int)posX][(int)(posY + planeY * moveSpeed)];
            if(destValY == 0 || destValY == 5) posY += planeY * moveSpeed;
        }
        if (keys & KEY_B) { // Strafe Derecha
            int destValX = worldMap[(int)(posX - planeX * moveSpeed)][(int)posY];
            if(destValX == 0 || destValX == 5) posX -= planeX * moveSpeed;

            int destValY = worldMap[(int)posX][(int)(posY - planeY * moveSpeed)];
            if(destValY == 0 || destValY == 5) posY -= planeY * moveSpeed;
        }

        if (keys & KEY_L) moveSpeed = 0.05;
        else if (keys & KEY_R) moveSpeed = 0.2;
        else moveSpeed = 0.1;

        if (keysDown() & KEY_START) break;

        // Verificar victoria
        if((int)posX == 17 && (int)posY == 6 && worldMap[17][6] == 5) {
            if(!gameWon) {
                gameWon = true;
                showVictory = true;
                frameCount = 0; 
                continue;
            }
        }


        for(int i = 0; i < 32*24; i++) {
            mapBuffer[i] = 0; // Fondo azul consistente
        }


        for(int x = 0; x < 32; x++) {
            double cameraX = 2 * x / (double)(32) - 1;
            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;

            int mapX = (int)posX;
            int mapY = (int)posY;

            double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
            double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
            double perpWallDist;

            double sideDistX, sideDistY;
            int stepX, stepY;
            int hit = 0;
            int side;

            if (rayDirX < 0) {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0 - posX) * deltaDistX;
            }
            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
            }

            while (hit == 0) {
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                if (worldMap[mapX][mapY] > 0) hit = 1;
            }

            if(side == 0) perpWallDist = (sideDistX - deltaDistX);
            else perpWallDist = (sideDistY - deltaDistY);

            int lineHeight = (int)(18.0 / perpWallDist);
            if(lineHeight < 1) lineHeight = 1;

            int drawStart = 10 - lineHeight / 2;
            int drawEnd = 10 + lineHeight / 2;
            
            if(drawStart < 1) drawStart = 1;
            if(drawEnd > 22) drawEnd = 22;

        
            int colorTile = 1;
            int wallType = worldMap[mapX][mapY];
            
            if(side == 0) {

                colorTile = wallType;
            } else {

                colorTile = wallType + 7;
            }

            // Efecto de niebla suave
            if(perpWallDist > fogDistance) {
                colorTile = 15; // Lejano
            } else if (perpWallDist > fogDistance * 0.75) {
                colorTile = 6; // Intermedio
            }


            for(int y = 0; y < 24; y++) {
                if(y == 0) continue; 
                
                if(y < drawStart) {
                    if (y < 8) mapBuffer[y * 32 + x] = 0; // Azul oscuro
                    else mapBuffer[y * 32 + x] = 6;       // Gris horizonte
                }
                else if(y >= drawStart && y <= drawEnd) {
                    // Pared
                    mapBuffer[y * 32 + x] = colorTile;
                }
                else {
                    // Sueloe
                    if (y > 18) mapBuffer[y * 32 + x] = 6; // Gris oscuro
                    else mapBuffer[y * 32 + x] = 7;        // Gris claro
                }
            }
        }


        // Barra de velocidad
        for(int x = 0; x < 32; x++) {
            if(x < (int)(moveSpeed * 160)) {
                if(keys & KEY_R) mapBuffer[0 * 32 + x] = 1;
                else if(keys & KEY_L) mapBuffer[0 * 32 + x] = 2;
                else mapBuffer[0 * 32 + x] = 5;
            } else {
                mapBuffer[0 * 32 + x] = 6;
            }
        }

        // Dibujar fondo y marco del minimapa
        for(int my = 16; my < 24; my++) {
            for(int mx = 20; mx < 32; mx++) {
                if(my == 16 || my == 23 || mx == 20 || mx == 31) mapBuffer[my * 32 + mx] = 13;
                else mapBuffer[my * 32 + mx] = 0;
            }
        }
        
        // Dibujar contenido del minimapa
        for(int my = 0; my < 6; my++) {
            for(int mx = 0; mx < 10; mx++) {
                int worldX = (int)posX - 5 + mx;
                int worldY = (int)posY - 3 + my;
                if(worldX >= 0 && worldX < mapWidth && worldY >= 0 && worldY < mapHeight) {
                    int mapTile = worldMap[worldX][worldY];
                    int displayTile = (mapTile > 0) ? mapTile : 7;
                    if(mapTile > 5) displayTile = 6;
                    mapBuffer[(17 + my) * 32 + (21 + mx)] = displayTile;
                }
            }
        }
        
        // Dibujar jugador (punto amarillo)
        int playerMiniMapX = 21 + 5;
        int playerMiniMapY = 17 + 3;
        mapBuffer[playerMiniMapY * 32 + playerMiniMapX] = 14;

        //  Dibujar indicador de dirección (punto rojo) 
        int dirX_mini = playerMiniMapX + (int)(dirX * 2);
        int dirY_mini = playerMiniMapY + (int)(dirY * 2);
        
     
        if(dirX_mini > 21 && dirX_mini < 31 && dirY_mini > 17 && dirY_mini < 23) {
            mapBuffer[dirY_mini * 32 + dirX_mini] = 1; // Tile rojo
        }

        fpsCounter++;
        if(frameCount / 60 > lastSecond) {
            displayFPS = fpsCounter;
            fpsCounter = 0;
            lastSecond = frameCount / 60;
        }

        if(frameCount % 20 == 0) {
            iprintf("\x1b[8;0H=== DS RAYCASTER ENHANCED ===");
            iprintf("\x1b[9;0H Pos: %.1f,%.1f           ", posX, posY);
            iprintf("\x1b[12;0H FPS: %d               ", displayFPS);
            iprintf("\x1b[13;0H Vida: %d/%d           ", playerHP, maxHP);
            iprintf("\x1b[14;0H Objetivo: Sala Dorada    ");
        }

        frameCount++;
        
        swiWaitForVBlank();
    }

    return 0;
}
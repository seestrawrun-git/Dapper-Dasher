#include "raylib.h"
#include <string>

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    //update running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
    }

    //update animation frame

    data.rec.x = data.frame * data.rec.width;
    if(data.runningTime == 0.0)
    {
        data.frame++;
    }
    
    if(data.frame > maxFrame)
    {
        data.frame = 0;
    }

    return data;
}


bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}



int main()
{
    //window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 1024;
    windowDimensions[1] = 768;
    const float textureScale = 4.0;




    int velocity{0};

    
    //initialize window
    InitWindow(windowDimensions[0], windowDimensions[1], "Spaghetti");
    SetTargetFPS(60);

    bool collision{};
    bool WinCollision{};
    //acceleration due to dravity(pixels/second/second)
    const int gravity{1000};


    //textures

    //Scarfy tex
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;




    
    //nebula tex
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    //background textures
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};

    //midground texture
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    
    //foreground texture
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};



    //AnimData for nebula

    const int sizeOfNebulae{2};

    AnimData nebulae[sizeOfNebulae]{};

    for(int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0/16.0;
        nebulae[i].pos.x = windowDimensions[0] + (i*300);

    }


    float finishLine{ nebulae[sizeOfNebulae -1 ].pos.x };




    //nebula X velocity (pixels/second)
    int nebulaVelocity{-200};


    //jump velocity (pixels per second)
    const int jumpVelocity{-600};


    //player states

    bool isInAir{};


    //debug
    std::string NumAsString;


     
    while(!WindowShouldClose())
    {
        //delta time (time since last frame)
        const float dT{GetFrameTime()};
        //start drawing
        BeginDrawing();
        ClearBackground(BLUE);
        bgX -= 100 * dT;
        mgX -= 200 *dT;
        if(mgX <= -background.width*textureScale)
        {
            mgX = 0;
        }


        fgX -= 300 * dT;

        if(fgX <= - foreground.width*textureScale)
        {
            fgX = 0;
        }

        if(bgX <= -background.width*textureScale)
        {
            bgX = 0;
        }

        //draw the background
        Vector2 bg1Pos{bgX, 0.0};
        Vector2 bg2Pos{bgX + background.width*4, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, textureScale, WHITE);
        DrawTextureEx(background, bg2Pos, 0.0, textureScale, WHITE);
        Vector2 mg1Pos{mgX, 0.0};
        Vector2 mg2Pos{mgX + midground.width*4, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, textureScale, WHITE);
        DrawTextureEx(midground, mg2Pos, 0.0, textureScale, WHITE);
        Vector2 fg1Pos{fgX, 0.0};
        Vector2 fg2Pos{fgX + foreground.width*4, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, textureScale, WHITE);
        DrawTextureEx(foreground, fg2Pos, 0.0, textureScale, WHITE);


        //NumAsString = std::to_string(scarfyData.pos.y);

        //DrawText(NumAsString.c_str(), windowDimensions[0]/2, windowDimensions[1]/2, 32, RED);
        
        //update animation frame scarfy

        scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;

        //update animation frame nebula

        for(int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }
        


        //update each nebula position
        for(int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i].pos.x += nebulaVelocity * dT;
        }
        //update finishline
        finishLine += nebulaVelocity * dT;
        

        
        
        //update runningTime
        //scarfyData.runningTime+=dT;

        //collision checks
        

        for(AnimData nebula : nebulae)
        {
            float pad{40.0};
            Rectangle nebRec{
            nebula.pos.x + pad,
            nebula.pos.y + pad,
            nebula.rec.width - 2 * pad,
            nebula.rec.height - 2 * pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

        if(!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, dT, 5);

        }
        
        //draw each nebula

        if(collision){
            //lose
            DrawText("Game Over", windowDimensions[0]/4, windowDimensions[1]/2, 46, RED);
        }
        else if(scarfyData.pos.x >= finishLine)
        {
            DrawText("You Win", windowDimensions[0]/4, windowDimensions[1]/2, 46, RED);
        }
        else
        {
            for(int i = 0; i < sizeOfNebulae; i++)
        {
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
        }
         //draw scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        //win condition

 


   


        //perform ground check
        if (isOnGround(scarfyData, windowDimensions[1]))
        {   
            //rectangle is on ground
            velocity = 0;
            isInAir = false;

        }
        else
        {
            //rectangle is in air
            velocity += gravity * dT;
            isInAir = true;
        }

        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVelocity;
        }


        //update position
        scarfyData.pos.y += velocity * dT;


        //stop drawing
        EndDrawing();


    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();

}


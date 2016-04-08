#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    srand((unsigned)time(NULL));//初始化随机数种子
    visibleSize = Director::getInstance()->getVisibleSize();
    gameLayer = Node::create();
    addChild(gameLayer);
    
    timeLabel = Label::create();//时间标签
    timeLabel->setTextColor(Color4B::MAGENTA);
    timeLabel->setSystemFontName("Menlo");
    timeLabel->setSystemFontSize(48);
    timeLabel->setPosition(visibleSize.width/2,visibleSize.height-100);
    addChild(timeLabel);
    startGame();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch * t,Event * e)
    {
        auto bs = GameBlock::getBlocks();
        GameBlock * b;
        
        for (auto it = bs->begin(); it!=bs->end(); it++) {
            b=*it;
            if (b->getLineIndex()==1&&
                b->getBoundingBox().containsPoint(t->getLocation())) {
                if (b->getColor()==Color3B::BLACK) {
                    if (!timeRunning) {
                        this->startTimer();
                    }
                    b->setColor(Color3B::GRAY);
                    this->moveDown();
                }
                else if (b->getColor()==Color3B::GREEN)
                {
                    this->moveDown();
                    this->stopTimer();
                }
                else
                {
                    MessageBox("你点错了", "点错了");
                    b->setColor(Color3B::RED);
                }
                break;
            }
        }
        return false;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void HelloWorld::startGame()
{
    log("start");
    //初始化数据
    stopTimer();
    linesCount = 0;
    showEnd = false;
    timeRunning = false;
    currentEndLine = NULL;
    timeLabel->setString("0.0000");
    
    //try to clear
    GameBlock::removeAllBlocks();
    addStartLine();
    addNormalLine(1);
    addNormalLine(2);
    addNormalLine(3);
}
void HelloWorld::update(float dt)
{
    long offset = clock()-startTime;
    timeLabel->setString(StringUtils::format("%g",((double)offset)/1000000));
}
void HelloWorld::startTimer()
{
    if (!timeRunning) {
        scheduleUpdate();
        startTime = clock();
        
        timeRunning = true;
    }
}
void HelloWorld::stopTimer()
{
    if (timeRunning) {
        unscheduleUpdate();
        
        timeRunning = false;
    }
}
void HelloWorld::addEndLine()
{
    auto b = EndLine::createWithContext(this);
    b->setLineIndex(4);
    b->setPositionY(b->getLineIndex()*visibleSize.height/4);
    gameLayer->addChild(b);
    
    currentEndLine = b;
}
void HelloWorld::addNormalLine(int lineIndex)
{
    GameBlock * b;
    int blackIndex = rand()%4;
    
    for (int i = 0; i<4; i++) {
        b = GameBlock::createWithArgs(blackIndex==i?Color3B::BLACK:Color3B::WHITE, Size(visibleSize.width/4-1, visibleSize.height/4-1), "", 20, Color4B::BLACK);
        gameLayer->addChild(b);
        b->setPosition(i*visibleSize.width/4,lineIndex*visibleSize.height/4);
        b->setLineIndex(lineIndex);
    }
    linesCount++;
}
void HelloWorld::addStartLine()
{
    auto b = GameBlock::createWithArgs(Color3B::YELLOW, Size(visibleSize.width,visibleSize.height/4), "", 20, Color4B::BLACK);
    gameLayer->addChild(b);
    b->setLineIndex(0);
}
void HelloWorld::moveDown()
{
    if (linesCount<50) {
        addNormalLine(4);
    }
    else if (!showEnd)
    {
        addEndLine();
        showEnd = true;
    }
    
    auto bs = GameBlock::getBlocks();
    for (auto it=bs->begin(); it!=bs->end(); it++) {
        (*it)->moveDown();
    }
    if (currentEndLine!=NULL) {
        if (currentEndLine->getLineIndex()==1) {
            // game end
            moveDown();
            stopTimer();
        }
    }
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

#include "SimpleAudioEngine.h"
#include "PhysicsShapeCache.h"
#include "GameScene.h"
#include "ui/CocosGUI.h":


USING_NS_CC;


Scene* GameScene::createScene()
{
    return GameScene::create();
}



bool GameScene::init()
{
    if ( !Scene::Scene::initWithPhysics() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Device::setAccelerometerEnabled(true);
	auto accListener = EventListenerAcceleration::create(CC_CALLBACK_2(GameScene::OnAcceleration, this));
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(accListener, this);
    
	this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	this->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize);
	
	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	edgeNode->setPhysicsBody(edgeBody);
	
	//edgeBody->setCategoryBitmask(0x03);
	//edgeBody->setCollisionBitmask(0x01);
	addChild(edgeNode);

	auto shapeCache = PhysicsShapeCache::getInstance();
	shapeCache->addShapesWithFile("tankBodies.plist");
	{
		 tankSprite = Sprite::create("Tank12.png");
		shapeCache->setBodyOnSprite("Tank12Body", tankSprite);
		tankSprite->setPosition(400, 600);
		tankSprite->setScale(200 / tankSprite->getContentSize().width);

		addChild(tankSprite);
	}
	{
		auto sprite = Sprite::create("Tank11.png");
		shapeCache->setBodyOnSprite("Tank11Body", sprite);
		sprite->setPosition(600, 400);		
		sprite->setScale(200 / sprite->getContentSize().width);

		addChild(sprite);
	}
	{
		auto sprite = Sprite::create("Tank10.png");
		shapeCache->setBodyOnSprite("Tank10Body", sprite);		
		sprite->setScale(200 / sprite->getContentSize().width);

		sprite->setPosition(400, 400);
		addChild(sprite);
	}
	{
		auto sprite = Sprite::create("Tank9.png");
		shapeCache->setBodyOnSprite("Tank9Body", sprite);		
		sprite->setScale(200 / sprite->getContentSize().width);

		sprite->setPosition(700, 700);
		addChild(sprite);
	}

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto listener1 = EventListenerTouchOneByOne::create();

	// trigger when you push down
	listener1->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan,this);

	// trigger when moving touch
	//listener1->onTouchMoved = [](Touch * touch, Event * event) {
	//	// your code
	//};

	// trigger when you let up
	listener1->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	// Add listener
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

	auto button = ui::Button::create("CloseNormal.png", "CloseSelected.png");
	button->setPosition(Point(200, 200));
	button->addTouchEventListener([&](Ref * sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			brake = true;
			break;
		case ui::Widget::TouchEventType::ENDED:
			brake = false;

			break;
		default:
			break;
		}
		});

	this->addChild(button);

	auto button1 = ui::Button::create("CloseNormal.png", "CloseSelected.png");
	button1->setPosition(Point(1700, 200));
	button1->addTouchEventListener([&](Ref * sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
		{
			auto bullet = Sprite::create("CloseNormal.png");
			bullet->setScale(0.4);
			bullet->setPosition( tankSprite->getPosition() );
			auto bulletBody = PhysicsBody::createCircle(bullet->getContentSize().width / 2);
			bullet->setPhysicsBody(bulletBody);
			bulletBody->applyForce(Vec2(10000000, 0));
			addChild(bullet);
		}
			
			break;
		case ui::Widget::TouchEventType::ENDED:

			break;
		default:
			break;
		}
		});

	this->addChild(button1);

	scheduleUpdate();
    return true;
}
bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	accelerate = true;
	
		return true; // if you are consuming it
};

void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
	accelerate = false;

}

void GameScene::OnAcceleration(cocos2d::Acceleration* acc, cocos2d::Event* event) {
	//CCLOG("accboi %f %f %f", acc->z, acc->y, acc->x);
	CCLOG("torque %f", acc->y * 1000000000000);
	tankSprite->getPhysicsBody()->applyTorque(-1*acc->x*3000000000);
}

// Implementation of the keyboard event callback function prototype
void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	log("Key with keycode %d pressed", keyCode);
	switch ((int) keyCode) {
	case 146:	tankSprite->getPhysicsBody()->applyImpulse(Vec2(10000000, 0));
		break;
	case 142:	tankSprite->getPhysicsBody()->applyImpulse(Vec2(-10000000, 0));
		break;
	case 124:	tankSprite->getPhysicsBody()->applyTorque(10000000000);
		break;
	case 127:	tankSprite->getPhysicsBody()->applyTorque(-10000000000);
		break;
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	log("Key with keycode %d released", keyCode);
}

void GameScene::update(float dt) {
	if(accelerate)
	tankSprite->getPhysicsBody()->applyForce(Vec2(80000000, 0));

	if(brake)	tankSprite->getPhysicsBody()->applyForce(Vec2(-20000000, 0));

}
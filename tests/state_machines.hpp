void load_state_machines(){
    return;
using namespace constrained_based_networks;
auto pool = Pool::getInstance();
	try{
	auto sm = new StateMachine("Main::LawnMoverOverPipe",pool);
	auto start_state = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("heading","1.5707963267948966");
	 start_state->addConfig("depth","-4");
	 start_state->addConfig("delta_timeout","10");
	 start_state->addConfig("x","-5");
	 start_state->addConfig("y","0");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("depth","-4");
	 source->addConfig("delta_timeout","10");
	 source->addConfig("x","-5");
	 source->addConfig("y","0");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
        /*
        printf("1 Hallo state from target: %s\n",(pool->getComponent("AuvControl::SimplePosMove")->getName().c_str()));
        printf("2 Hallo state from target: %s\n",(target)->getName().c_str());
        printf("2.5 Hallo state from target: %s\n",(target->foo()->getName().c_str()));
        printf("3 Hallo state from target: %s\n",(dynamic_cast<Component*>(target))->getName().c_str());
        */
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("depth","-4");
	 target->addConfig("delta_timeout","10");
	 target->addConfig("x","-5");
	 target->addConfig("y","4");
	auto trigger = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("depth","-4");
	 trigger->addConfig("delta_timeout","10");
	 trigger->addConfig("x","-5");
	 trigger->addConfig("y","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("depth","-4");
	 source->addConfig("delta_timeout","10");
	 source->addConfig("x","-5");
	 source->addConfig("y","4");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","0");
	 target->addConfig("depth","-4");
	 target->addConfig("delta_timeout","5");
	auto trigger = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("depth","-4");
	 trigger->addConfig("delta_timeout","10");
	 trigger->addConfig("x","-5");
	 trigger->addConfig("y","4");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("depth","-4");
	 source->addConfig("delta_timeout","5");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","0");
	 target->addConfig("depth","-4");
	 target->addConfig("delta_timeout","10");
	 target->addConfig("x","0");
	 target->addConfig("y","4");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-4");
	 trigger->addConfig("delta_timeout","5");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("depth","-4");
	 source->addConfig("delta_timeout","10");
	 source->addConfig("x","0");
	 source->addConfig("y","4");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","-1.5707963267948966");
	 target->addConfig("depth","-4");
	 target->addConfig("delta_timeout","5");
	auto trigger = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-4");
	 trigger->addConfig("delta_timeout","10");
	 trigger->addConfig("x","0");
	 trigger->addConfig("y","4");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","-1.5707963267948966");
	 source->addConfig("depth","-4");
	 source->addConfig("delta_timeout","5");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("depth","-4");
	 target->addConfig("delta_timeout","10");
	 target->addConfig("x","0");
	 target->addConfig("y","0");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","-1.5707963267948966");
	 trigger->addConfig("depth","-4");
	 trigger->addConfig("delta_timeout","5");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("depth","-4");
	 source->addConfig("delta_timeout","10");
	 source->addConfig("x","0");
	 source->addConfig("y","0");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","0");
	 target->addConfig("depth","-4");
	 target->addConfig("delta_timeout","5");
	auto trigger = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("depth","-4");
	 trigger->addConfig("delta_timeout","10");
	 trigger->addConfig("x","0");
	 trigger->addConfig("y","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("depth","-4");
	 source->addConfig("delta_timeout","5");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","0");
	 target->addConfig("depth","-4");
	 target->addConfig("delta_timeout","10");
	 target->addConfig("x","5");
	 target->addConfig("y","0");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-4");
	 trigger->addConfig("delta_timeout","5");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("depth","-4");
	 source->addConfig("delta_timeout","10");
	 source->addConfig("x","5");
	 source->addConfig("y","0");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("depth","-4");
	 target->addConfig("delta_timeout","5");
	auto trigger = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-4");
	 trigger->addConfig("delta_timeout","10");
	 trigger->addConfig("x","5");
	 trigger->addConfig("y","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("depth","-4");
	 source->addConfig("delta_timeout","5");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("depth","-4");
	 target->addConfig("delta_timeout","10");
	 target->addConfig("x","5");
	 target->addConfig("y","4");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("depth","-4");
	 trigger->addConfig("delta_timeout","5");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvControl::SimplePosMove");}
	try{
	auto sm = new StateMachine("Main::BlindForwardAndBack",pool);
	auto start_state = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 start_state->addConfig("timeout","var:time");
	 start_state->addConfig("heading","var:heading");
	 start_state->addConfig("speed_x","var:speed");
	 start_state->addConfig("depth","var:depth");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("timeout","var:time");
	 source->addConfig("heading","var:heading");
	 source->addConfig("speed_x","var:speed");
	 source->addConfig("depth","var:depth");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("timeout","5");
	 target->addConfig("heading","var:heading");
	 target->addConfig("depth","var:depth");
	 target->addConfig("speed_x","0");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("timeout","var:time");
	 trigger->addConfig("heading","var:heading");
	 trigger->addConfig("speed_x","var:speed");
	 trigger->addConfig("depth","var:depth");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("timeout","5");
	 source->addConfig("heading","var:heading");
	 source->addConfig("depth","var:depth");
	 source->addConfig("speed_x","0");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("timeout","15");
	 target->addConfig("heading","#<struct Roby::Tools::Calculus::Binary op=:+, left=#<struct Roby::Coordination::Models::Variable name=:heading>, right=3.141592653589793>");
	 target->addConfig("depth","var:depth");
	 target->addConfig("speed_x","0");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("timeout","5");
	 trigger->addConfig("heading","var:heading");
	 trigger->addConfig("depth","var:depth");
	 trigger->addConfig("speed_x","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("timeout","15");
	 source->addConfig("heading","#<struct Roby::Tools::Calculus::Binary op=:+, left=#<struct Roby::Coordination::Models::Variable name=:heading>, right=3.141592653589793>");
	 source->addConfig("depth","var:depth");
	 source->addConfig("speed_x","0");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("timeout","var:time");
	 target->addConfig("heading","#<struct Roby::Tools::Calculus::Binary op=:+, left=#<struct Roby::Coordination::Models::Variable name=:heading>, right=3.141592653589793>");
	 target->addConfig("speed_x","var:speed");
	 target->addConfig("depth","var:depth");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("timeout","15");
	 trigger->addConfig("heading","#<struct Roby::Tools::Calculus::Binary op=:+, left=#<struct Roby::Coordination::Models::Variable name=:heading>, right=3.141592653589793>");
	 trigger->addConfig("depth","var:depth");
	 trigger->addConfig("speed_x","0");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::MoveCmp");}
	new StateMachine("Main::IntelligentFollowPipe",pool);
	try{
	auto sm = new StateMachine("Main::FollowPipeATurnAtEOfPipe",pool);
	auto start_state = pool->getComponent("Main::IntelligentFollowPipe")->getSpecialized();
	 start_state->addConfig("turn_dir","var:turn_dir");
	 start_state->addConfig("initial_heading","var:initial_heading");
	 start_state->addConfig("precision","0.5");
sm->setStart(start_state);
{
	auto source = pool->getComponent("Main::IntelligentFollowPipe")->getSpecialized();
	 source->addConfig("turn_dir","var:turn_dir");
	 source->addConfig("initial_heading","var:initial_heading");
	 source->addConfig("precision","0.5");
	auto target = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 target->addConfig("heading","var:initial_heading");
	 target->addConfig("speed_x","-0.25");
	 target->addConfig("turn_dir","var:turn_dir");
	 target->addConfig("timeout","10");
	auto trigger = pool->getComponent("Main::IntelligentFollowPipe")->getSpecialized();
	 trigger->addConfig("turn_dir","var:turn_dir");
	 trigger->addConfig("initial_heading","var:initial_heading");
	 trigger->addConfig("precision","0.5");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("heading","var:initial_heading");
	 source->addConfig("speed_x","-0.25");
	 source->addConfig("turn_dir","var:turn_dir");
	 source->addConfig("timeout","10");
	auto target = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 target->addConfig("heading","var:post_heading");
	 target->addConfig("speed_x","0");
	 target->addConfig("turn_dir","var:turn_dir");
	 target->addConfig("timeout","10");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("heading","var:initial_heading");
	 trigger->addConfig("speed_x","-0.25");
	 trigger->addConfig("turn_dir","var:turn_dir");
	 trigger->addConfig("timeout","10");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get Main::IntelligentFollowPipe");}
	try{
	auto sm = new StateMachine("Main::PipePingPong",pool);
	auto start_state = pool->getComponent("Main::FollowPipeATurnAtEOfPipe")->getSpecialized();
	 start_state->addConfig("turn_dir","1");
	 start_state->addConfig("initial_heading","0");
	 start_state->addConfig("post_heading","3.13");
sm->setStart(start_state);
{
	auto source = pool->getComponent("Main::FollowPipeATurnAtEOfPipe")->getSpecialized();
	 source->addConfig("turn_dir","1");
	 source->addConfig("initial_heading","0");
	 source->addConfig("post_heading","3.13");
	auto target = pool->getComponent("Main::FollowPipeATurnAtEOfPipe")->getSpecialized();
	 target->addConfig("turn_dir","1");
	 target->addConfig("initial_heading","3.13");
	 target->addConfig("post_heading","var:post_heading");
	auto trigger = pool->getComponent("Main::FollowPipeATurnAtEOfPipe")->getSpecialized();
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("initial_heading","0");
	 trigger->addConfig("post_heading","3.13");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get Main::FollowPipeATurnAtEOfPipe");}
	try{
	auto sm = new StateMachine("Main::LoopPipePingPong",pool);
	auto start_state = pool->getComponent("Main::PipePingPong")->getSpecialized();
	 start_state->addConfig("post_heading","0");
sm->setStart(start_state);
{
	auto source = pool->getComponent("Main::PipePingPong")->getSpecialized();
	 source->addConfig("post_heading","0");
	auto target = pool->getComponent("Main::PipePingPong")->getSpecialized();
	 target->addConfig("post_heading","0");
	auto trigger = pool->getComponent("Main::PipePingPong")->getSpecialized();
	 trigger->addConfig("post_heading","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::PipePingPong")->getSpecialized();
	 source->addConfig("post_heading","0");
	auto target = pool->getComponent("Main::PipePingPong")->getSpecialized();
	 target->addConfig("post_heading","0");
	auto trigger = pool->getComponent("Main::PipePingPong")->getSpecialized();
	 trigger->addConfig("post_heading","0");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get Main::PipePingPong");}
	try{
	auto sm = new StateMachine("Main::Simple",pool);
	auto start_state = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 start_state->addConfig("heading","0");
	 start_state->addConfig("depth","-5");
	 start_state->addConfig("timeout","15");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("heading","0");
	 source->addConfig("depth","-5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("heading","0");
	 target->addConfig("speed_x","3");
	 target->addConfig("depth","-5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","3");
	 source->addConfig("depth","-5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("speed_x","3");
	 target->addConfig("depth","-5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","3");
	 trigger->addConfig("depth","-5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("speed_x","3");
	 source->addConfig("depth","-5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("speed_x","3");
	 target->addConfig("depth","-5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("speed_x","3");
	 trigger->addConfig("depth","-5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("speed_x","3");
	 source->addConfig("depth","-5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("heading","4.71238898038469");
	 target->addConfig("speed_x","3");
	 target->addConfig("depth","-5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("speed_x","3");
	 trigger->addConfig("depth","-5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("heading","4.71238898038469");
	 source->addConfig("speed_x","3");
	 source->addConfig("depth","-5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("heading","0");
	 target->addConfig("speed_x","0");
	 target->addConfig("depth","-5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("heading","4.71238898038469");
	 trigger->addConfig("speed_x","3");
	 trigger->addConfig("depth","-5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvControl::SimpleMove");}
	try{
	auto sm = new StateMachine("Main::WallAndBuoy",pool);
	auto start_state = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 start_state->addConfig("timeout","300");
	 start_state->addConfig("corners","1");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 source->addConfig("timeout","300");
	 source->addConfig("corners","1");
	auto target = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 target->addConfig("timeout","180");
	 target->addConfig("corners","1");
	auto trigger = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 trigger->addConfig("timeout","300");
	 trigger->addConfig("corners","1");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::WorldXYPositionCmp");}
	try{
	auto sm = new StateMachine("Main::WallContinue",pool);
	auto start_state = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 start_state->addConfig("timeout","30");
	 start_state->addConfig("corners","1");
sm->setStart(start_state);
	}catch(...){printf("cannot (2) get AuvCont::WorldXYPositionCmp");}
	try{
	auto sm = new StateMachine("Main::BuoyWall",pool);
	auto start_state = pool->getComponent("Main::WallAndBuoy");
sm->setStart(start_state);
{
	auto source = pool->getComponent("Main::WallAndBuoy");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("x_speed","0");
	 target->addConfig("y_speed","0");
	 target->addConfig("timeout","5");
	auto trigger = pool->getComponent("Main::WallAndBuoy");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get Main::WallAndBuoy");}
	try{
	auto sm = new StateMachine("Main::WallBuoy",pool);
	auto start_state = pool->getComponent("Main::WallAndBuoy");
sm->setStart(start_state);
{
	auto source = pool->getComponent("Main::WallAndBuoy");
	auto target = pool->getComponent("AuvCont::WorldXYZPositionCmp");
	auto trigger = pool->getComponent("Main::WallAndBuoy");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get Main::WallAndBuoy");}
	try{
	auto sm = new StateMachine("Main::DiveAndLocalize",pool);
	auto start_state = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 start_state->addConfig("heading","0");
	 start_state->addConfig("depth","-5");
	 start_state->addConfig("timeout","15");
sm->setStart(start_state);
	}catch(...){printf("cannot (2) get AuvControl::SimpleMove");}
	try{
	auto sm = new StateMachine("Main::ToWindow",pool);
	auto start_state = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("heading","0");
	 start_state->addConfig("depth","-5.5");
	 start_state->addConfig("delta_timeout","10");
	 start_state->addConfig("x","7");
	 start_state->addConfig("y","6.5");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("depth","-5.5");
	 source->addConfig("delta_timeout","10");
	 source->addConfig("x","7");
	 source->addConfig("y","6.5");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","0");
	 target->addConfig("depth","-5.5");
	 target->addConfig("delta_timeout","120");
	 target->addConfig("x","8");
	 target->addConfig("y","6.5");
	auto trigger = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-5.5");
	 trigger->addConfig("delta_timeout","10");
	 trigger->addConfig("x","7");
	 trigger->addConfig("y","6.5");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvControl::SimplePosMove");}
	new StateMachine("Main::FixMapHack",pool);
	try{
	auto sm = new StateMachine("Main::SearchBlackbox",pool);
	auto start_state = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 start_state->addConfig("timeout","4");
	 start_state->addConfig("heading","0");
	 start_state->addConfig("speed_x","0");
	 start_state->addConfig("depth","-1");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("timeout","4");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0");
	 source->addConfig("depth","-1");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("timeout","4");
	 target->addConfig("heading","0");
	 target->addConfig("speed_x","3");
	 target->addConfig("depth","-1");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("timeout","4");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0");
	 trigger->addConfig("depth","-1");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("timeout","4");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","3");
	 source->addConfig("depth","-1");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("timeout","4");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("speed_x","0");
	 target->addConfig("depth","-1");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("timeout","4");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","3");
	 trigger->addConfig("depth","-1");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("timeout","4");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("speed_x","0");
	 source->addConfig("depth","-1");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("timeout","4");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("speed_x","3");
	 target->addConfig("depth","-1");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("timeout","4");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("speed_x","0");
	 trigger->addConfig("depth","-1");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("timeout","4");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("speed_x","3");
	 source->addConfig("depth","-1");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("timeout","4");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("speed_x","0");
	 target->addConfig("depth","-1");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("timeout","4");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("speed_x","3");
	 trigger->addConfig("depth","-1");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("timeout","4");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("speed_x","0");
	 source->addConfig("depth","-1");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("timeout","4");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("speed_x","3");
	 target->addConfig("depth","-1");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("timeout","4");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("speed_x","0");
	 trigger->addConfig("depth","-1");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("timeout","4");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("speed_x","3");
	 source->addConfig("depth","-1");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("timeout","4");
	 target->addConfig("heading","4.71238898038469");
	 target->addConfig("speed_x","0");
	 target->addConfig("depth","-1");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("timeout","4");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("speed_x","3");
	 trigger->addConfig("depth","-1");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("timeout","4");
	 source->addConfig("heading","4.71238898038469");
	 source->addConfig("speed_x","0");
	 source->addConfig("depth","-1");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("timeout","4");
	 target->addConfig("heading","4.71238898038469");
	 target->addConfig("speed_x","3");
	 target->addConfig("depth","-1");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("timeout","4");
	 trigger->addConfig("heading","4.71238898038469");
	 trigger->addConfig("speed_x","0");
	 trigger->addConfig("depth","-1");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::MoveCmp");}
	try{
	auto sm = new StateMachine("Main::ExploreMap",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("depth","-1.0");
	 start_state->addConfig("delta_timeout","1");
	 start_state->addConfig("x","-30");
	 start_state->addConfig("y","10.0");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-1.0");
	 source->addConfig("delta_timeout","1");
	 source->addConfig("x","-30");
	 source->addConfig("y","10.0");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-1.0");
	 target->addConfig("delta_timeout","1");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("x","-30");
	 target->addConfig("y","40.0");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-1.0");
	 trigger->addConfig("delta_timeout","1");
	 trigger->addConfig("x","-30");
	 trigger->addConfig("y","10.0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-1.0");
	 source->addConfig("delta_timeout","1");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("x","-30");
	 source->addConfig("y","40.0");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-1.0");
	 target->addConfig("delta_timeout","1");
	 target->addConfig("x","-10");
	 target->addConfig("y","40.0");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-1.0");
	 trigger->addConfig("delta_timeout","1");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("x","-30");
	 trigger->addConfig("y","40.0");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::InspectStructure",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("depth","-3.0");
	 start_state->addConfig("delta_timeout","3");
	 start_state->addConfig("x","-18.5");
	 start_state->addConfig("y","28");
	 start_state->addConfig("heading","3.141592653589793");
	 start_state->addConfig("timeout","30");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-18.5");
	 source->addConfig("y","28");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-18.5");
	 target->addConfig("y","23");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-18.5");
	 trigger->addConfig("y","28");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-18.5");
	 source->addConfig("y","23");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-18.5");
	 target->addConfig("y","23");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-18.5");
	 trigger->addConfig("y","23");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-18.5");
	 source->addConfig("y","23");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-23.5");
	 target->addConfig("y","23");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-18.5");
	 trigger->addConfig("y","23");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-23.5");
	 source->addConfig("y","23");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-23.5");
	 target->addConfig("y","23");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-23.5");
	 trigger->addConfig("y","23");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-23.5");
	 source->addConfig("y","23");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-23.5");
	 target->addConfig("y","28");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-23.5");
	 trigger->addConfig("y","23");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-23.5");
	 source->addConfig("y","28");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-23.5");
	 target->addConfig("y","28");
	 target->addConfig("heading","-1.5707963267948966");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-23.5");
	 trigger->addConfig("y","28");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-23.5");
	 source->addConfig("y","28");
	 source->addConfig("heading","-1.5707963267948966");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-18.5");
	 target->addConfig("y","28");
	 target->addConfig("heading","-1.5707963267948966");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-23.5");
	 trigger->addConfig("y","28");
	 trigger->addConfig("heading","-1.5707963267948966");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-18.5");
	 source->addConfig("y","28");
	 source->addConfig("heading","-1.5707963267948966");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-19");
	 target->addConfig("y","27.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-18.5");
	 trigger->addConfig("y","28");
	 trigger->addConfig("heading","-1.5707963267948966");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-19");
	 source->addConfig("y","27.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-19");
	 target->addConfig("y","23.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-19");
	 trigger->addConfig("y","27.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-19");
	 source->addConfig("y","23.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-19");
	 target->addConfig("y","23.5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-19");
	 trigger->addConfig("y","23.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-19");
	 source->addConfig("y","23.5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-23");
	 target->addConfig("y","23.5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-19");
	 trigger->addConfig("y","23.5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-23");
	 source->addConfig("y","23.5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-23");
	 target->addConfig("y","23.5");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-23");
	 trigger->addConfig("y","23.5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-23");
	 source->addConfig("y","23.5");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-23");
	 target->addConfig("y","27.5");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-23");
	 trigger->addConfig("y","23.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-23");
	 source->addConfig("y","27.5");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-23");
	 target->addConfig("y","27.5");
	 target->addConfig("heading","-1.5707963267948966");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-23");
	 trigger->addConfig("y","27.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-23");
	 source->addConfig("y","27.5");
	 source->addConfig("heading","-1.5707963267948966");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-3.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-19");
	 target->addConfig("y","27.5");
	 target->addConfig("heading","-1.5707963267948966");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-23");
	 trigger->addConfig("y","27.5");
	 trigger->addConfig("heading","-1.5707963267948966");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-3.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-19");
	 source->addConfig("y","27.5");
	 source->addConfig("heading","-1.5707963267948966");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-20");
	 target->addConfig("y","26");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-3.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-19");
	 trigger->addConfig("y","27.5");
	 trigger->addConfig("heading","-1.5707963267948966");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-20");
	 source->addConfig("y","26");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-20");
	 target->addConfig("y","24");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-20");
	 trigger->addConfig("y","26");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-20");
	 source->addConfig("y","24");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-20");
	 target->addConfig("y","23");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-20");
	 trigger->addConfig("y","24");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-20");
	 source->addConfig("y","23");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-22");
	 target->addConfig("y","24");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-20");
	 trigger->addConfig("y","23");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-22");
	 source->addConfig("y","24");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-22");
	 target->addConfig("y","24");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","24");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-22");
	 source->addConfig("y","24");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-22");
	 target->addConfig("y","26");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","24");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-22");
	 source->addConfig("y","26");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-22");
	 target->addConfig("y","26");
	 target->addConfig("heading","-1.5707963267948966");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","26");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2.0");
	 source->addConfig("delta_timeout","3");
	 source->addConfig("x","-22");
	 source->addConfig("y","26");
	 source->addConfig("heading","-1.5707963267948966");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2.0");
	 target->addConfig("delta_timeout","3");
	 target->addConfig("x","-20");
	 target->addConfig("y","26");
	 target->addConfig("heading","-1.5707963267948966");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2.0");
	 trigger->addConfig("delta_timeout","3");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","26");
	 trigger->addConfig("heading","-1.5707963267948966");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::FindPipeWithLocalization",pool);
	auto start_state = pool->getComponent("Pipeline::Detector");
sm->setStart(start_state);
	}catch(...){printf("cannot (2) get Pipeline::Detector");}
	try{
	auto sm = new StateMachine("Main::ResetHeadingOnWall",pool);
	auto start_state = pool->getComponent("AuvCont::WorldXYPositionCmp");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::WorldXYPositionCmp");
	auto target = pool->getComponent("PoseAuv::InitialOrientationEstimatorCmp");
	auto trigger = pool->getComponent("Wall::DetectorNew");
	sm->addTransition(source,target,trigger,"wall_servoing");
}
	}catch(...){printf("cannot (2) get AuvCont::WorldXYPositionCmp");}
	try{
	auto sm = new StateMachine("Main::PingPongPipeWallBackToPipe",pool);
	auto start_state = pool->getComponent("Main::PipePingPong");
sm->setStart(start_state);
{
	auto source = pool->getComponent("Main::PipePingPong");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("max_corners","2");
	auto trigger = pool->getComponent("Main::PipePingPong");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("max_corners","2");
	auto target = pool->getComponent("Main::FindPipeWithLocalization");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("max_corners","2");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get Main::PipePingPong");}
	try{
	auto sm = new StateMachine("Main::PingPongPipeWallBackToPipeWithWindow",pool);
	auto start_state = pool->getComponent("Main::PipePingPong");
sm->setStart(start_state);
{
	auto source = pool->getComponent("Main::PipePingPong");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("max_corners","2");
	auto trigger = pool->getComponent("Main::PipePingPong");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("max_corners","2");
	auto target = pool->getComponent("Main::ToWindow");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("max_corners","2");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::ToWindow");
	auto target = pool->getComponent("Main::FindPipeWithLocalization");
	auto trigger = pool->getComponent("Main::ToWindow");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get Main::PipePingPong");}
	try{
	auto sm = new StateMachine("Main::Rocking",pool);
	auto start_state = pool->getComponent("Main::PingPongPipeWallBackToPipe");
sm->setStart(start_state);
{
	auto source = pool->getComponent("Main::PingPongPipeWallBackToPipe");
	auto target = pool->getComponent("Main::PingPongPipeWallBackToPipe");
	auto trigger = pool->getComponent("Main::PingPongPipeWallBackToPipe");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::PingPongPipeWallBackToPipe");
	auto target = pool->getComponent("Main::PingPongPipeWallBackToPipe");
	auto trigger = pool->getComponent("Main::PingPongPipeWallBackToPipe");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get Main::PingPongPipeWallBackToPipe");}
	try{
	auto sm = new StateMachine("Main::MinimalDemoOnce",pool);
	auto start_state = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("heading","0");
	 start_state->addConfig("depth","-4");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("timeout","15");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("depth","-4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("Main::FindPipeWithLocalization");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::FindPipeWithLocalization");
	auto target = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 target->addConfig("depth","-5.5");
	 target->addConfig("heading","0");
	 target->addConfig("speed_x","0.5");
	 target->addConfig("turn_dir","1");
	 target->addConfig("timeout","180");
	auto trigger = pool->getComponent("Main::FindPipeWithLocalization");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::FindPipeWithLocalization");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("depth","-5");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("x","0.5");
	 target->addConfig("y","5.5");
	auto trigger = pool->getComponent("Main::FindPipeWithLocalization");
	sm->addTransition(source,target,trigger,"failed");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-5.5");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","180");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.14");
	 target->addConfig("depth","-5");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-5.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","180");
	sm->addTransition(source,target,trigger,"end_of_pipe");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-5.5");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","180");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.14");
	 target->addConfig("depth","-5");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-5.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","180");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-5.5");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","180");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("depth","-5");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("x","0.5");
	 target->addConfig("y","5.5");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-5.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","180");
	sm->addTransition(source,target,trigger,"lost_pipe");
}
{
	auto source = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("depth","-5");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("x","0.5");
	 source->addConfig("y","5.5");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("max_corners","1");
	auto trigger = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("depth","-5");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("x","0.5");
	 trigger->addConfig("y","5.5");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","3.14");
	 source->addConfig("depth","-5");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("max_corners","1");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","3.14");
	 trigger->addConfig("depth","-5");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("max_corners","1");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("timeout","20");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("max_corners","1");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("timeout","20");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","0");
	 target->addConfig("depth","1");
	 target->addConfig("speed_x","0.1");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("timeout","20");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvControl::SimpleMove");}
	try{
	auto sm = new StateMachine("Main::MinimalDemo",pool);
	auto start_state = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("heading","0");
	 start_state->addConfig("depth","-7");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("timeout","15");
	 start_state->addConfig("speed_x","0");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("depth","-7");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","15");
	 source->addConfig("speed_x","0");
	auto target = pool->getComponent("Main::FindPipeWithLocalization")->getSpecialized();
	 target->addConfig("check_pipe_angle","true");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-7");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","15");
	 trigger->addConfig("speed_x","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::FindPipeWithLocalization")->getSpecialized();
	 source->addConfig("check_pipe_angle","true");
	auto target = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 target->addConfig("depth","-7.1");
	 target->addConfig("heading","0");
	 target->addConfig("speed_x","0.5");
	 target->addConfig("turn_dir","1");
	 target->addConfig("timeout","120");
	auto trigger = pool->getComponent("Main::FindPipeWithLocalization")->getSpecialized();
	 trigger->addConfig("check_pipe_angle","true");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::FindPipeWithLocalization")->getSpecialized();
	 source->addConfig("check_pipe_angle","true");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("depth","-6");
	 target->addConfig("delta_timeout","20");
	 target->addConfig("x","0.5");
	 target->addConfig("y","5.5");
	 target->addConfig("speed_x","0");
	auto trigger = pool->getComponent("Main::FindPipeWithLocalization")->getSpecialized();
	 trigger->addConfig("check_pipe_angle","true");
	sm->addTransition(source,target,trigger,"failed");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-7.1");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","120");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("depth","-6");
	 target->addConfig("delta_timeout","20");
	 target->addConfig("x","0.5");
	 target->addConfig("y","5.5");
	 target->addConfig("speed_x","0");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-7.1");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","120");
	sm->addTransition(source,target,trigger,"end_of_pipe");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-7.1");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","120");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("depth","-6");
	 target->addConfig("delta_timeout","20");
	 target->addConfig("x","0.5");
	 target->addConfig("y","5.5");
	 target->addConfig("speed_x","0");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-7.1");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","120");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-7.1");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","120");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("depth","-6");
	 target->addConfig("delta_timeout","20");
	 target->addConfig("x","0.5");
	 target->addConfig("y","5.5");
	 target->addConfig("speed_x","0");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-7.1");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","120");
	sm->addTransition(source,target,trigger,"failed");
}
{
	auto source = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("depth","-6");
	 source->addConfig("delta_timeout","20");
	 source->addConfig("x","0.5");
	 source->addConfig("y","5.5");
	 source->addConfig("speed_x","0");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("max_corners","1");
	auto trigger = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("depth","-6");
	 trigger->addConfig("delta_timeout","20");
	 trigger->addConfig("x","0.5");
	 trigger->addConfig("y","5.5");
	 trigger->addConfig("speed_x","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("max_corners","1");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("max_corners","1");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("heading","0.0");
	 target->addConfig("depth","-6.0");
	 target->addConfig("timeout","5");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("heading","0.0");
	 source->addConfig("depth","-6.0");
	 source->addConfig("timeout","5");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("heading","0.0");
	 target->addConfig("depth","-6.0");
	 target->addConfig("timeout","5");
	 target->addConfig("speed_x","0.15");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("heading","0.0");
	 trigger->addConfig("depth","-6.0");
	 trigger->addConfig("timeout","5");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("heading","0.0");
	 source->addConfig("depth","-6.0");
	 source->addConfig("timeout","5");
	 source->addConfig("speed_x","0.15");
	auto target = pool->getComponent("Main::FindPipeWithLocalization")->getSpecialized();
	 target->addConfig("check_pipe_angle","true");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("heading","0.0");
	 trigger->addConfig("depth","-6.0");
	 trigger->addConfig("timeout","5");
	 trigger->addConfig("speed_x","0.15");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvControl::SimpleMove");}
	try{
	auto sm = new StateMachine("Main::MinimalDemoBlind",pool);
	auto start_state = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("heading","0");
	 start_state->addConfig("depth","-4");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("timeout","15");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("depth","-4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvControl::TrajectoryMove")->getSpecialized();
	 target->addConfig("target","over_pipeline");
	 target->addConfig("timeout","125");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::TrajectoryMove")->getSpecialized();
	 source->addConfig("target","over_pipeline");
	 source->addConfig("timeout","125");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("max_corners","1");
	auto trigger = pool->getComponent("AuvControl::TrajectoryMove")->getSpecialized();
	 trigger->addConfig("target","over_pipeline");
	 trigger->addConfig("timeout","125");
	sm->addTransition(source,target,trigger,"reached_end");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("max_corners","1");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("timeout","23");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("max_corners","1");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("timeout","23");
	auto target = pool->getComponent("AuvControl::TrajectoryMove")->getSpecialized();
	 target->addConfig("target","over_pipeline");
	 target->addConfig("timeout","125");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("timeout","23");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvControl::SimpleMove");}
	try{
	auto sm = new StateMachine("Main::FullDemo",pool);
	auto start_state = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("heading","0");
	 start_state->addConfig("depth","-6");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("timeout","15");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("depth","-6");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("Main::FindPipeWithLocalization");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-6");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::FindPipeWithLocalization");
	auto target = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 target->addConfig("depth","-5.5");
	 target->addConfig("heading","0");
	 target->addConfig("speed_x","0.5");
	 target->addConfig("turn_dir","1");
	 target->addConfig("timeout","180");
	auto trigger = pool->getComponent("Main::FindPipeWithLocalization");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-5.5");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","180");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.14");
	 target->addConfig("depth","-6");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-5.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","180");
	sm->addTransition(source,target,trigger,"end_of_pipe");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-5.5");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","180");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.14");
	 target->addConfig("depth","-6");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-5.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","180");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","3.14");
	 source->addConfig("depth","-6");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("max_corners","1");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","3.14");
	 trigger->addConfig("depth","-6");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("max_corners","1");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("timeout","20");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("max_corners","1");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("timeout","20");
	auto target = pool->getComponent("Main::FindPipeWithLocalization");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("timeout","20");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::FindPipeWithLocalization");
	auto target = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 target->addConfig("depth","-5.5");
	 target->addConfig("heading","0");
	 target->addConfig("speed_x","0.5");
	 target->addConfig("turn_dir","1");
	 target->addConfig("timeout","180");
	auto trigger = pool->getComponent("Main::FindPipeWithLocalization");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-5.5");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","180");
	auto target = pool->getComponent("AuvControl::TrajectoryMove")->getSpecialized();
	 target->addConfig("target","explore");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-5.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","180");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-5.5");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","180");
	auto target = pool->getComponent("AuvControl::TrajectoryMove")->getSpecialized();
	 target->addConfig("target","explore");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-5.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","180");
	sm->addTransition(source,target,trigger,"end_of_pipe");
}
{
	auto source = pool->getComponent("AuvControl::TrajectoryMove")->getSpecialized();
	 source->addConfig("target","explore");
	auto target = pool->getComponent("Main::FindPipeWithLocalization");
	auto trigger = pool->getComponent("AuvControl::TrajectoryMove")->getSpecialized();
	 trigger->addConfig("target","explore");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvControl::SimpleMove");}
	try{
	auto sm = new StateMachine("Main::TrajectoryDemo",pool);
	auto start_state = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("heading","0");
	 start_state->addConfig("depth","-7");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("timeout","15");
	 start_state->addConfig("speed_x","0");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("depth","-7");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","15");
	 source->addConfig("speed_x","0");
	auto target = pool->getComponent("AuvControl::TrajectoryMove");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-7");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","15");
	 trigger->addConfig("speed_x","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::TrajectoryMove");
	auto target = pool->getComponent("Main::FindPipeWithLocalization");
	auto trigger = pool->getComponent("AuvControl::TrajectoryMove");
	sm->addTransition(source,target,trigger,"reached_end");
}
{
	auto source = pool->getComponent("Main::FindPipeWithLocalization");
	auto target = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 target->addConfig("depth","-7");
	 target->addConfig("heading","0");
	 target->addConfig("speed_x","0.5");
	 target->addConfig("turn_dir","1");
	 target->addConfig("timeout","120");
	auto trigger = pool->getComponent("Main::FindPipeWithLocalization");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-7");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","120");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("depth","-6");
	 target->addConfig("delta_timeout","20");
	 target->addConfig("x","0.5");
	 target->addConfig("y","5.5");
	 target->addConfig("speed_x","0");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-7");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","120");
	sm->addTransition(source,target,trigger,"end_of_pipe");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-7");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","120");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("depth","-6");
	 target->addConfig("delta_timeout","20");
	 target->addConfig("x","0.5");
	 target->addConfig("y","5.5");
	 target->addConfig("speed_x","0");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-7");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","120");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-7");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","120");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("depth","-6");
	 target->addConfig("delta_timeout","20");
	 target->addConfig("x","0.5");
	 target->addConfig("y","5.5");
	 target->addConfig("speed_x","0");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-7");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","120");
	sm->addTransition(source,target,trigger,"lost_pipe");
}
{
	auto source = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("depth","-6");
	 source->addConfig("delta_timeout","20");
	 source->addConfig("x","0.5");
	 source->addConfig("y","5.5");
	 source->addConfig("speed_x","0");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("max_corners","1");
	auto trigger = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("depth","-6");
	 trigger->addConfig("delta_timeout","20");
	 trigger->addConfig("x","0.5");
	 trigger->addConfig("y","5.5");
	 trigger->addConfig("speed_x","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("max_corners","1");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("timeout","23");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("max_corners","1");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("timeout","23");
	auto target = pool->getComponent("AuvControl::TrajectoryMove");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("timeout","23");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvControl::SimpleMove");}
	try{
	auto sm = new StateMachine("Main::AdvancedDemo",pool);
	auto start_state = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("heading","0");
	 start_state->addConfig("depth","-7");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("timeout","15");
	 start_state->addConfig("speed_x","0");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("depth","-7");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","15");
	 source->addConfig("speed_x","0");
	auto target = pool->getComponent("Main::FindPipeWithLocalization");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-7");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","15");
	 trigger->addConfig("speed_x","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::FindPipeWithLocalization");
	auto target = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 target->addConfig("depth","-7");
	 target->addConfig("heading","0");
	 target->addConfig("speed_x","0.5");
	 target->addConfig("turn_dir","1");
	 target->addConfig("timeout","120");
	auto trigger = pool->getComponent("Main::FindPipeWithLocalization");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::FindPipeWithLocalization");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","0");
	 target->addConfig("depth","-7");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("x","0.5");
	 target->addConfig("y","5.5");
	 target->addConfig("speed_x","0");
	 target->addConfig("timeout","40");
	auto trigger = pool->getComponent("Main::FindPipeWithLocalization");
	sm->addTransition(source,target,trigger,"failed");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-7");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","120");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","-0.6283185307179586");
	 target->addConfig("depth","-7");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("x","9.5");
	 target->addConfig("y","0");
	 target->addConfig("speed_x","1");
	 target->addConfig("timeout","60");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-7");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","120");
	sm->addTransition(source,target,trigger,"end_of_pipe");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-7");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","120");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","-0.6283185307179586");
	 target->addConfig("depth","-7");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("x","9.5");
	 target->addConfig("y","0");
	 target->addConfig("speed_x","1");
	 target->addConfig("timeout","60");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-7");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","120");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-7");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","120");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","0");
	 target->addConfig("depth","-7");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("x","0.5");
	 target->addConfig("y","5.5");
	 target->addConfig("speed_x","0");
	 target->addConfig("timeout","40");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-7");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","120");
	sm->addTransition(source,target,trigger,"failed");
}
{
	auto source = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","-0.6283185307179586");
	 source->addConfig("depth","-7");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("x","9.5");
	 source->addConfig("y","0");
	 source->addConfig("speed_x","1");
	 source->addConfig("timeout","60");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("depth","-7");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","-0.6283185307179586");
	 trigger->addConfig("depth","-7");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("x","9.5");
	 trigger->addConfig("y","0");
	 trigger->addConfig("speed_x","1");
	 trigger->addConfig("timeout","60");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("depth","-7");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("max_corners","2");
	 target->addConfig("timeout","120");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("depth","-7");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("depth","-7");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("x","0.5");
	 source->addConfig("y","5.5");
	 source->addConfig("speed_x","0");
	 source->addConfig("timeout","40");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","-0.6283185307179586");
	 target->addConfig("depth","-7");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("x","9.5");
	 target->addConfig("y","0");
	 target->addConfig("speed_x","1");
	 target->addConfig("timeout","60");
	auto trigger = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-7");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("x","0.5");
	 trigger->addConfig("y","5.5");
	 trigger->addConfig("speed_x","0");
	 trigger->addConfig("timeout","40");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("max_corners","2");
	 source->addConfig("timeout","120");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("max_corners","2");
	 trigger->addConfig("timeout","120");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("heading","1.0471975511965976");
	 target->addConfig("depth","-7.0");
	 target->addConfig("timeout","5");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("heading","1.0471975511965976");
	 source->addConfig("depth","-7.0");
	 source->addConfig("timeout","5");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("heading","1.0471975511965976");
	 target->addConfig("depth","-7.0");
	 target->addConfig("timeout","15");
	 target->addConfig("speed_x","0.15");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("heading","1.0471975511965976");
	 trigger->addConfig("depth","-7.0");
	 trigger->addConfig("timeout","5");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("heading","1.0471975511965976");
	 source->addConfig("depth","-7.0");
	 source->addConfig("timeout","15");
	 source->addConfig("speed_x","0.15");
	auto target = pool->getComponent("Main::FindPipeWithLocalization");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("heading","1.0471975511965976");
	 trigger->addConfig("depth","-7.0");
	 trigger->addConfig("timeout","15");
	 trigger->addConfig("speed_x","0.15");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvControl::SimpleMove");}
	try{
	auto sm = new StateMachine("Main::AdvancedDemoOnce",pool);
	auto start_state = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("heading","0");
	 start_state->addConfig("depth","-7");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("timeout","15");
	 start_state->addConfig("speed_x","0");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("depth","-7");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","15");
	 source->addConfig("speed_x","0");
	auto target = pool->getComponent("Main::FindPipeWithLocalization");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-7");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","15");
	 trigger->addConfig("speed_x","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::FindPipeWithLocalization");
	auto target = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 target->addConfig("depth","-7.1");
	 target->addConfig("heading","0");
	 target->addConfig("speed_x","0.5");
	 target->addConfig("turn_dir","1");
	 target->addConfig("timeout","120");
	auto trigger = pool->getComponent("Main::FindPipeWithLocalization");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::FindPipeWithLocalization");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("depth","-6");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("x","0.5");
	 target->addConfig("y","5.5");
	 target->addConfig("speed_x","0");
	auto trigger = pool->getComponent("Main::FindPipeWithLocalization");
	sm->addTransition(source,target,trigger,"failed");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-7.1");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","120");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.14");
	 target->addConfig("depth","-6");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-7.1");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","120");
	sm->addTransition(source,target,trigger,"end_of_pipe");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-7.1");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","120");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.14");
	 target->addConfig("depth","-6");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-7.1");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","120");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 source->addConfig("depth","-7.1");
	 source->addConfig("heading","0");
	 source->addConfig("speed_x","0.5");
	 source->addConfig("turn_dir","1");
	 source->addConfig("timeout","120");
	auto target = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("depth","-6");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("x","0.5");
	 target->addConfig("y","5.5");
	 target->addConfig("speed_x","0");
	auto trigger = pool->getComponent("Pipeline::Follower")->getSpecialized();
	 trigger->addConfig("depth","-7.1");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("speed_x","0.5");
	 trigger->addConfig("turn_dir","1");
	 trigger->addConfig("timeout","120");
	sm->addTransition(source,target,trigger,"failed");
}
{
	auto source = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("depth","-6");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("x","0.5");
	 source->addConfig("y","5.5");
	 source->addConfig("speed_x","0");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("max_corners","1");
	auto trigger = pool->getComponent("AuvControl::SimplePosMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("depth","-6");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("x","0.5");
	 trigger->addConfig("y","5.5");
	 trigger->addConfig("speed_x","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","3.14");
	 source->addConfig("depth","-6");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("max_corners","1");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","3.14");
	 trigger->addConfig("depth","-6");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("max_corners","1");
	auto target = pool->getComponent("Wall::Follower")->getSpecialized();
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("max_corners","1");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Wall::Follower")->getSpecialized();
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("heading","0.0");
	 target->addConfig("depth","-6.0");
	 target->addConfig("timeout","5");
	auto trigger = pool->getComponent("Wall::Follower")->getSpecialized();
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("heading","0.0");
	 source->addConfig("depth","-6.0");
	 source->addConfig("timeout","5");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("heading","0.0");
	 target->addConfig("depth","-6.0");
	 target->addConfig("timeout","5");
	 target->addConfig("speed_x","0.15");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("heading","0.0");
	 trigger->addConfig("depth","-6.0");
	 trigger->addConfig("timeout","5");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvControl::SimpleMove");}
	try{
	auto sm = new StateMachine("Main::SearchStructure",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("timeout","300");
	 start_state->addConfig("depth","-7");
	 start_state->addConfig("x","-45.0");
	 start_state->addConfig("y","25");
	 start_state->addConfig("heading","1.5707963267948966");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("timeout","300");
	 source->addConfig("depth","-7");
	 source->addConfig("x","-45.0");
	 source->addConfig("y","25");
	 source->addConfig("heading","1.5707963267948966");
	auto target = pool->getComponent("AuvCont::StructureCmp");
	auto trigger = pool->getComponent("Structure::Alignment");
	sm->addTransition(source,target,trigger,"aligning");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::FindBlackbox",pool);
	auto start_state = pool->getComponent("Main::ExploreMap");
sm->setStart(start_state);
{
	auto source = pool->getComponent("Main::ExploreMap");
	auto target = pool->getComponent("Main::FixMapHack");
	auto trigger = pool->getComponent("Main::ExploreMap");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::FixMapHack");
	auto target = pool->getComponent("AuvCont::WorldPositionCmp");
	auto trigger = pool->getComponent("Main::FixMapHack");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::WorldPositionCmp");
	auto target = pool->getComponent("Main::SearchBlackbox");
	auto trigger = pool->getComponent("AuvCont::WorldPositionCmp");
	sm->addTransition(source,target,trigger,"reached_target");
}
{
	auto source = pool->getComponent("AuvCont::WorldPositionCmp");
	auto target = pool->getComponent("Main::FixMapHack");
	auto trigger = pool->getComponent("AuvCont::WorldPositionCmp");
	sm->addTransition(source,target,trigger,"servoing_finished");
}
{
	auto source = pool->getComponent("AuvCont::WorldPositionCmp");
	auto target = pool->getComponent("Main::FixMapHack");
	auto trigger = pool->getComponent("AuvCont::WorldPositionCmp");
	sm->addTransition(source,target,trigger,"not_enough_targets");
}
{
	auto source = pool->getComponent("Main::SearchBlackbox");
	auto target = pool->getComponent("AuvCont::WorldPositionCmp");
	auto trigger = pool->getComponent("Main::SearchBlackbox");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::FixMapHack");
	auto target = pool->getComponent("Main::ExploreMap");
	auto trigger = pool->getComponent("Main::FixMapHack");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::SearchBlackbox");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","0.0");
	 target->addConfig("delta_timeout","5");
	auto trigger = pool->getComponent("Buoy::DetectorCmp2");
	sm->addTransition(source,target,trigger,"buoy_detected");
}
	}catch(...){printf("cannot (2) get Main::ExploreMap");}
	try{
	auto sm = new StateMachine("Main::GateWithoutLocalization",pool);
	auto start_state = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("depth","-7");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("heading","1.5707963267948966");
	 start_state->addConfig("timeout","60");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-7");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","60");
	auto target = pool->getComponent("Main::SearchStructure");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-7");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","60");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::SearchStructure");
	auto target = pool->getComponent("Main::BlindForwardAndBack")->getSpecialized();
	 target->addConfig("time","20");
	 target->addConfig("speed","1.0");
	 target->addConfig("heading","-3.141592653589793");
	 target->addConfig("depth","-4");
	auto trigger = pool->getComponent("Main::SearchStructure");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::MoveCmp");}
	try{
	auto sm = new StateMachine("Main::WallWithLocalization",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("heading","1.5707963267948966");
	 start_state->addConfig("depth","-1.5");
	 start_state->addConfig("delta_timeout","2");
	 start_state->addConfig("x","-3");
	 start_state->addConfig("y","26.5");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("depth","-1.5");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-3");
	 source->addConfig("y","26.5");
	auto target = pool->getComponent("Main::BuoyWall");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-3");
	 trigger->addConfig("y","26.5");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::StructureInspection",pool);
	auto start_state = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("depth","-9");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("heading","1.5707963267948966");
	 start_state->addConfig("x","-15");
	 start_state->addConfig("timeout","20");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-9");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("x","-15");
	 source->addConfig("timeout","20");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("x","-22.5");
	 target->addConfig("y","25");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","120");
	 target->addConfig("depth","-2");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-9");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("x","-15");
	 trigger->addConfig("timeout","20");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","-22.5");
	 source->addConfig("y","25");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","120");
	 source->addConfig("depth","-2");
	auto target = pool->getComponent("AuvCont::StructureCmp")->getSpecialized();
	 target->addConfig("timeout","60");
	auto trigger = pool->getComponent("Structure::Detector");
	sm->addTransition(source,target,trigger,"servoing");
}
	}catch(...){printf("cannot (2) get AuvCont::MoveCmp");}
	try{
	auto sm = new StateMachine("Main::BlindQuali",pool);
	auto start_state = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("heading","1.5707963267948966");
	 start_state->addConfig("depth","-2");
	 start_state->addConfig("x_speed","1");
	 start_state->addConfig("timeout","10");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("depth","-2");
	 source->addConfig("x_speed","1");
	 source->addConfig("timeout","10");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("x","-22");
	 target->addConfig("y","25");
	 target->addConfig("timeout","150");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("x_speed","1");
	 trigger->addConfig("timeout","10");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("x","-22");
	 source->addConfig("y","25");
	 source->addConfig("timeout","150");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("heading","0");
	 target->addConfig("x","-22");
	 target->addConfig("y","25");
	 target->addConfig("timeout","30");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","25");
	 trigger->addConfig("timeout","150");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("heading","0");
	 source->addConfig("x","-22");
	 source->addConfig("y","25");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-1.5");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("heading","0.22");
	 target->addConfig("x","-5");
	 target->addConfig("y","26.5");
	 target->addConfig("timeout","60");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","25");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvControl::SimpleMove");}
	try{
	auto sm = new StateMachine("Main::TargetWallBuoyWall",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("depth","-1.5");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("heading","0.22");
	 start_state->addConfig("x","-5");
	 start_state->addConfig("y","26.5");
	 start_state->addConfig("timeout","60");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-1.5");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("heading","0.22");
	 source->addConfig("x","-5");
	 source->addConfig("y","26.5");
	 source->addConfig("timeout","60");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-1.5");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("heading","1.57");
	 target->addConfig("x","-5");
	 target->addConfig("y","26.5");
	 target->addConfig("timeout","60");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("heading","0.22");
	 trigger->addConfig("x","-5");
	 trigger->addConfig("y","26.5");
	 trigger->addConfig("timeout","60");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-1.5");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("heading","1.57");
	 source->addConfig("x","-5");
	 source->addConfig("y","26.5");
	 source->addConfig("timeout","60");
	auto target = pool->getComponent("Main::WallAndBuoy");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("heading","1.57");
	 trigger->addConfig("x","-5");
	 trigger->addConfig("y","26.5");
	 trigger->addConfig("timeout","60");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::WallAndBuoy");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("x_speed","0");
	 target->addConfig("y_speed","0");
	 target->addConfig("timeout","5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("depth","-1.5");
	auto trigger = pool->getComponent("Main::WallAndBuoy");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("x_speed","0");
	 source->addConfig("y_speed","0");
	 source->addConfig("timeout","5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("depth","-1.5");
	auto target = pool->getComponent("Main::WallContinue");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("x_speed","0");
	 trigger->addConfig("y_speed","0");
	 trigger->addConfig("timeout","5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("depth","-1.5");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::WallContinue");
	auto target = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 target->addConfig("timeout","20");
	auto trigger = pool->getComponent("Main::WallContinue");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 source->addConfig("timeout","20");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("heading","-2.356194490192345");
	 target->addConfig("x","-22");
	 target->addConfig("y","25");
	 target->addConfig("timeout","150");
	auto trigger = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 trigger->addConfig("timeout","20");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::Win",pool);
	auto start_state = pool->getComponent("Main::BlindQuali");
sm->setStart(start_state);
{
	auto source = pool->getComponent("Main::BlindQuali");
	auto target = pool->getComponent("Main::TargetWallBuoyWall");
	auto trigger = pool->getComponent("Main::BlindQuali");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::TargetWallBuoyWall");
	auto target = pool->getComponent("Main::InspectStructure");
	auto trigger = pool->getComponent("Main::TargetWallBuoyWall");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get Main::BlindQuali");}
	try{
	auto sm = new StateMachine("Main::WandBoje",pool);
	auto start_state = pool->getComponent("Main::WallWithLocalization");
sm->setStart(start_state);
	}catch(...){printf("cannot (2) get Main::WallWithLocalization");}
	try{
	auto sm = new StateMachine("Main::WandBojeJudge",pool);
	auto start_state = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("heading","0");
	 start_state->addConfig("depth","-2");
	 start_state->addConfig("timeout","8");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("depth","-2");
	 source->addConfig("timeout","8");
	auto target = pool->getComponent("Main::WallWithLocalization");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("timeout","8");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvControl::SimpleMove");}
	try{
	auto sm = new StateMachine("Main::QualiBoje",pool);
	auto start_state = pool->getComponent("Main::BlindQuali");
sm->setStart(start_state);
{
	auto source = pool->getComponent("Main::BlindQuali");
	auto target = pool->getComponent("Main::WallWithLocalization");
	auto trigger = pool->getComponent("Main::BlindQuali");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::WallWithLocalization");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-1.5");
	 target->addConfig("delta_timeout","40");
	 target->addConfig("heading","-2.199114857512855");
	 target->addConfig("x","-22");
	 target->addConfig("y","25");
	 target->addConfig("timeout","3");
	auto trigger = pool->getComponent("Main::WallWithLocalization");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get Main::BlindQuali");}
	try{
	auto sm = new StateMachine("Main::OnlyWall",pool);
	auto start_state = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 start_state->addConfig("timeout","240");
	 start_state->addConfig("max_corners","1");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 source->addConfig("timeout","240");
	 source->addConfig("max_corners","1");
	auto target = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 target->addConfig("timeout","20");
	auto trigger = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 trigger->addConfig("timeout","240");
	 trigger->addConfig("max_corners","1");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 source->addConfig("timeout","240");
	 source->addConfig("max_corners","1");
	auto target = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 target->addConfig("timeout","20");
	auto trigger = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 trigger->addConfig("timeout","240");
	 trigger->addConfig("max_corners","1");
	sm->addTransition(source,target,trigger,"failed");
}
	}catch(...){printf("cannot (2) get AuvCont::WorldXYPositionCmp");}
	try{
	auto sm = new StateMachine("Main::Wall",pool);
	auto start_state = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 start_state->addConfig("timeout","300");
	 start_state->addConfig("corners","1");
sm->setStart(start_state);
	}catch(...){printf("cannot (2) get AuvCont::WorldXYPositionCmp");}
	try{
	auto sm = new StateMachine("Main::WallBuoyWall",pool);
	auto start_state = pool->getComponent("Main::WallAndBuoy");
sm->setStart(start_state);
{
	auto source = pool->getComponent("Main::WallAndBuoy");
	auto target = pool->getComponent("AuvCont::WorldXYZPositionCmp");
	auto trigger = pool->getComponent("Main::WallAndBuoy");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::WorldXYZPositionCmp");
	auto target = pool->getComponent("Main::WallContinue");
	auto trigger = pool->getComponent("AuvCont::WorldXYZPositionCmp");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get Main::WallAndBuoy");}
	try{
	auto sm = new StateMachine("Main::TargetWall",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("depth","-1.5");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("heading","0.33");
	 start_state->addConfig("x","-5");
	 start_state->addConfig("y","26.5");
	 start_state->addConfig("timeout","60");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-1.5");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("heading","0.33");
	 source->addConfig("x","-5");
	 source->addConfig("y","26.5");
	 source->addConfig("timeout","60");
	auto target = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 target->addConfig("timeout","300");
	 target->addConfig("corners","1");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("heading","0.33");
	 trigger->addConfig("x","-5");
	 trigger->addConfig("y","26.5");
	 trigger->addConfig("timeout","60");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 source->addConfig("timeout","300");
	 source->addConfig("corners","1");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("heading","4.71238898038469");
	 target->addConfig("x","-22");
	 target->addConfig("y","25");
	 target->addConfig("timeout","150");
	auto trigger = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 trigger->addConfig("timeout","300");
	 trigger->addConfig("corners","1");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::TargetWallBuoy",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("depth","-1.5");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("heading","0.22");
	 start_state->addConfig("x","-5");
	 start_state->addConfig("y","26.5");
	 start_state->addConfig("timeout","60");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-1.5");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("heading","0.22");
	 source->addConfig("x","-5");
	 source->addConfig("y","26.5");
	 source->addConfig("timeout","60");
	auto target = pool->getComponent("Main::WallAndBuoy");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("heading","0.22");
	 trigger->addConfig("x","-5");
	 trigger->addConfig("y","26.5");
	 trigger->addConfig("timeout","60");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::WallAndBuoy");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("x_speed","0");
	 target->addConfig("y_speed","0");
	 target->addConfig("timeout","5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("depth","-1.5");
	auto trigger = pool->getComponent("Main::WallAndBuoy");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("x_speed","0");
	 source->addConfig("y_speed","0");
	 source->addConfig("timeout","5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("depth","-1.5");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("x","-22");
	 target->addConfig("y","25");
	 target->addConfig("timeout","150");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("x_speed","0");
	 trigger->addConfig("y_speed","0");
	 trigger->addConfig("timeout","5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("depth","-1.5");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::WallBuoyAsvWall",pool);
	auto start_state = pool->getComponent("Main::WallAndBuoy");
sm->setStart(start_state);
{
	auto source = pool->getComponent("Main::WallAndBuoy");
	auto target = pool->getComponent("AuvCont::WorldXYZPositionCmp");
	auto trigger = pool->getComponent("Main::WallAndBuoy");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::WorldXYZPositionCmp");
	auto target = pool->getComponent("AuvCont::MoveCmp");
	auto trigger = pool->getComponent("AuvCont::WorldXYZPositionCmp");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp");
	auto target = pool->getComponent("Main::WallContinue");
	auto trigger = pool->getComponent("AuvCont::MoveCmp");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get Main::WallAndBuoy");}
	try{
	auto sm = new StateMachine("Main::Structure",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("depth","-2");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("heading","1.5707963267948966");
	 start_state->addConfig("x","-22");
	 start_state->addConfig("y","25");
	 start_state->addConfig("timeout","150");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("x","-22");
	 source->addConfig("y","25");
	 source->addConfig("timeout","150");
	auto target = pool->getComponent("AuvCont::StructureCmp");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","25");
	 trigger->addConfig("timeout","150");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::StructureCmp");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("x","-22");
	 target->addConfig("y","25");
	 target->addConfig("timeout","5");
	auto trigger = pool->getComponent("AuvCont::StructureCmp");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::Box",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("depth","-2");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("heading","1.5707963267948966");
	 start_state->addConfig("x","-15");
	 start_state->addConfig("y","35");
	 start_state->addConfig("timeout","150");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("x","-15");
	 source->addConfig("y","35");
	 source->addConfig("timeout","150");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("x","-22");
	 target->addConfig("y","25");
	 target->addConfig("timeout","5");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("x","-15");
	 trigger->addConfig("y","35");
	 trigger->addConfig("timeout","150");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("x","-22");
	 source->addConfig("y","25");
	 source->addConfig("timeout","5");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("delta_x","-22");
	 target->addConfig("delta_y","25");
	 target->addConfig("timeout","5");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","25");
	 trigger->addConfig("timeout","5");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::BoxSearch",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("depth","-2");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("heading","1.5707963267948966");
	 start_state->addConfig("x","-15");
	 start_state->addConfig("y","35");
	 start_state->addConfig("timeout","150");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("x","-15");
	 source->addConfig("y","35");
	 source->addConfig("timeout","150");
	auto target = pool->getComponent("Buoy::DetectorCmp2");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("x","-15");
	 trigger->addConfig("y","35");
	 trigger->addConfig("timeout","150");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Buoy::DetectorCmp2");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("delta_x","-22");
	 target->addConfig("delta_y","25");
	 target->addConfig("timeout","5");
	auto trigger = pool->getComponent("Buoy::DetectorCmp2");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::Leak",pool);
	auto start_state = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("depth","-2");
	 start_state->addConfig("heading","2.356194490192345");
	 start_state->addConfig("x_speed","1");
	 start_state->addConfig("y_speed","0");
	 start_state->addConfig("timeout","30");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("heading","2.356194490192345");
	 source->addConfig("x_speed","1");
	 source->addConfig("y_speed","0");
	 source->addConfig("timeout","30");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("x","-11");
	 target->addConfig("y","12");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","60");
	 target->addConfig("heading","-1.5707963267948966");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("heading","2.356194490192345");
	 trigger->addConfig("x_speed","1");
	 trigger->addConfig("y_speed","0");
	 trigger->addConfig("timeout","30");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("x","-11");
	 source->addConfig("y","12");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","60");
	 source->addConfig("heading","-1.5707963267948966");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("x","-17");
	 target->addConfig("y","13");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","60");
	 target->addConfig("heading","3.141592653589793");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("x","-11");
	 trigger->addConfig("y","12");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","60");
	 trigger->addConfig("heading","-1.5707963267948966");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("x","-17");
	 source->addConfig("y","13");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","60");
	 source->addConfig("heading","3.141592653589793");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("x","-18");
	 target->addConfig("y","12");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","60");
	 target->addConfig("heading","2.356194490192345");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("x","-17");
	 trigger->addConfig("y","13");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","60");
	 trigger->addConfig("heading","3.141592653589793");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("x","-18");
	 source->addConfig("y","12");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","60");
	 source->addConfig("heading","2.356194490192345");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("x","-19");
	 target->addConfig("y","16");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","60");
	 target->addConfig("heading","1.5707963267948966");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("x","-18");
	 trigger->addConfig("y","12");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","60");
	 trigger->addConfig("heading","2.356194490192345");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("x","-19");
	 source->addConfig("y","16");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","60");
	 source->addConfig("heading","1.5707963267948966");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("x","-16");
	 target->addConfig("y","17");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","60");
	 target->addConfig("heading","0");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("x","-19");
	 trigger->addConfig("y","16");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","60");
	 trigger->addConfig("heading","1.5707963267948966");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("x","-16");
	 source->addConfig("y","17");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","60");
	 source->addConfig("heading","0");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("x","-22");
	 target->addConfig("y","16");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","60");
	 target->addConfig("heading","3.141592653589793");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("x","-16");
	 trigger->addConfig("y","17");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","60");
	 trigger->addConfig("heading","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("x","-22");
	 source->addConfig("y","16");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","60");
	 source->addConfig("heading","3.141592653589793");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("x","-22");
	 target->addConfig("y","19");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","60");
	 target->addConfig("heading","1.5707963267948966");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","16");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","60");
	 trigger->addConfig("heading","3.141592653589793");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("x","-22");
	 source->addConfig("y","19");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","60");
	 source->addConfig("heading","1.5707963267948966");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("x","-21");
	 target->addConfig("y","22");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","60");
	 target->addConfig("heading","1.5707963267948966");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","19");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","60");
	 trigger->addConfig("heading","1.5707963267948966");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("x","-21");
	 source->addConfig("y","22");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","60");
	 source->addConfig("heading","1.5707963267948966");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("x","-22");
	 target->addConfig("y","25");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","60");
	 target->addConfig("heading","1.5707963267948966");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("x","-21");
	 trigger->addConfig("y","22");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","60");
	 trigger->addConfig("heading","1.5707963267948966");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("x","-22");
	 source->addConfig("y","25");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","60");
	 source->addConfig("heading","1.5707963267948966");
	auto target = pool->getComponent("Main::InspectStructure");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","25");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","60");
	 trigger->addConfig("heading","1.5707963267948966");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::InspectStructure");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-4");
	 target->addConfig("x","-22");
	 target->addConfig("y","25");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("timeout","60");
	 target->addConfig("heading","-2.356194490192345");
	auto trigger = pool->getComponent("Main::InspectStructure");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-4");
	 source->addConfig("x","-22");
	 source->addConfig("y","25");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("timeout","60");
	 source->addConfig("heading","-2.356194490192345");
	auto target = pool->getComponent("AuvCont::StructureCmp");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-4");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","25");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("timeout","60");
	 trigger->addConfig("heading","-2.356194490192345");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::MoveCmp");}
	try{
	auto sm = new StateMachine("Main::Longrange",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("x","1");
	 start_state->addConfig("y","2");
	 start_state->addConfig("depth","2.5");
	 start_state->addConfig("delta_x","4");
	 start_state->addConfig("delta_y","4");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("finished_when_reached","true");
	 start_state->addConfig("heading","3.141592653589793");
	 start_state->addConfig("timeout","480");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","1");
	 source->addConfig("y","2");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","480");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","1");
	 trigger->addConfig("y","2");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","480");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("depth","0.5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("x","3");
	 target->addConfig("y","4");
	 target->addConfig("depth","2.5");
	 target->addConfig("delta_x","4");
	 target->addConfig("delta_y","4");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("finished_when_reached","true");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","900");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("depth","0.5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","3");
	 source->addConfig("y","4");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","900");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","3");
	 trigger->addConfig("y","4");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","900");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("depth","0.5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("x","5");
	 target->addConfig("y","6");
	 target->addConfig("depth","2.5");
	 target->addConfig("delta_x","4");
	 target->addConfig("delta_y","4");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("finished_when_reached","true");
	 target->addConfig("heading","-1.5707963267948966");
	 target->addConfig("timeout","480");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("depth","0.5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","5");
	 source->addConfig("y","6");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","-1.5707963267948966");
	 source->addConfig("timeout","480");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","5");
	 trigger->addConfig("y","6");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","-1.5707963267948966");
	 trigger->addConfig("timeout","480");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("depth","0.5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("x","7");
	 target->addConfig("y","8");
	 target->addConfig("depth","2.5");
	 target->addConfig("delta_x","4");
	 target->addConfig("delta_y","4");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("finished_when_reached","true");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","900");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("depth","0.5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","7");
	 source->addConfig("y","8");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","900");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","7");
	 trigger->addConfig("y","8");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","900");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("depth","0.5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("x","9");
	 target->addConfig("y","10");
	 target->addConfig("depth","2.5");
	 target->addConfig("delta_x","4");
	 target->addConfig("delta_y","4");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("finished_when_reached","true");
	 target->addConfig("heading","0.7853981633974483");
	 target->addConfig("timeout","480");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("depth","0.5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","9");
	 source->addConfig("y","10");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","0.7853981633974483");
	 source->addConfig("timeout","480");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","9");
	 trigger->addConfig("y","10");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","0.7853981633974483");
	 trigger->addConfig("timeout","480");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::LongrangeiFailsafe",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("x","1");
	 start_state->addConfig("y","2");
	 start_state->addConfig("depth","2.5");
	 start_state->addConfig("delta_x","4");
	 start_state->addConfig("delta_y","4");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("finished_when_reached","true");
	 start_state->addConfig("heading","3.141592653589793");
	 start_state->addConfig("timeout","480");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","1");
	 source->addConfig("y","2");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","480");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","1");
	 trigger->addConfig("y","2");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","480");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("depth","0.5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("x","3");
	 target->addConfig("y","4");
	 target->addConfig("depth","2.5");
	 target->addConfig("delta_x","4");
	 target->addConfig("delta_y","4");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("finished_when_reached","true");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","900");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("depth","0.5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","3");
	 source->addConfig("y","4");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","900");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","3");
	 trigger->addConfig("y","4");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","900");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("depth","0.5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("x","5");
	 target->addConfig("y","6");
	 target->addConfig("depth","2.5");
	 target->addConfig("delta_x","4");
	 target->addConfig("delta_y","4");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("finished_when_reached","true");
	 target->addConfig("heading","-1.5707963267948966");
	 target->addConfig("timeout","480");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("depth","0.5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","5");
	 source->addConfig("y","6");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","-1.5707963267948966");
	 source->addConfig("timeout","480");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","5");
	 trigger->addConfig("y","6");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","-1.5707963267948966");
	 trigger->addConfig("timeout","480");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("depth","0.5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("x","7");
	 target->addConfig("y","8");
	 target->addConfig("depth","2.5");
	 target->addConfig("delta_x","4");
	 target->addConfig("delta_y","4");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("finished_when_reached","true");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","900");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("depth","0.5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","7");
	 source->addConfig("y","8");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","900");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","7");
	 trigger->addConfig("y","8");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","900");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("depth","0.5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("x","9");
	 target->addConfig("y","10");
	 target->addConfig("depth","2.5");
	 target->addConfig("delta_x","4");
	 target->addConfig("delta_y","4");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("finished_when_reached","true");
	 target->addConfig("heading","0.7853981633974483");
	 target->addConfig("timeout","480");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("depth","0.5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","9");
	 source->addConfig("y","10");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","0.7853981633974483");
	 source->addConfig("timeout","480");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","9");
	 trigger->addConfig("y","10");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","0.7853981633974483");
	 trigger->addConfig("timeout","480");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::LongrangeiTest",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("x","-170");
	 start_state->addConfig("y","25");
	 start_state->addConfig("depth","2.5");
	 start_state->addConfig("delta_x","4");
	 start_state->addConfig("delta_y","4");
	 start_state->addConfig("delta_timeout","5");
	 start_state->addConfig("finished_when_reached","true");
	 start_state->addConfig("heading","3.141592653589793");
	 start_state->addConfig("timeout","480");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","-170");
	 source->addConfig("y","25");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","480");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","-170");
	 trigger->addConfig("y","25");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","480");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("depth","0.5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("x","-170");
	 target->addConfig("y","10");
	 target->addConfig("depth","2.5");
	 target->addConfig("delta_x","4");
	 target->addConfig("delta_y","4");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("finished_when_reached","true");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","900");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("depth","0.5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","-170");
	 source->addConfig("y","10");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","900");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","-170");
	 trigger->addConfig("y","10");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","900");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("depth","0.5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("x","-200");
	 target->addConfig("y","10");
	 target->addConfig("depth","2.5");
	 target->addConfig("delta_x","4");
	 target->addConfig("delta_y","4");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("finished_when_reached","true");
	 target->addConfig("heading","-1.5707963267948966");
	 target->addConfig("timeout","480");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("depth","0.5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","-200");
	 source->addConfig("y","10");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","-1.5707963267948966");
	 source->addConfig("timeout","480");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","-200");
	 trigger->addConfig("y","10");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","-1.5707963267948966");
	 trigger->addConfig("timeout","480");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("depth","0.5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("x","-200");
	 target->addConfig("y","40");
	 target->addConfig("depth","2.5");
	 target->addConfig("delta_x","4");
	 target->addConfig("delta_y","4");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("finished_when_reached","true");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","900");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("depth","0.5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","-200");
	 source->addConfig("y","40");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","900");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","-200");
	 trigger->addConfig("y","40");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","900");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("depth","0.5");
	 source->addConfig("timeout","15");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("x","-170");
	 target->addConfig("y","40");
	 target->addConfig("depth","2.5");
	 target->addConfig("delta_x","4");
	 target->addConfig("delta_y","4");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("finished_when_reached","true");
	 target->addConfig("heading","0.7853981633974483");
	 target->addConfig("timeout","480");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("depth","0.5");
	 trigger->addConfig("timeout","15");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("x","-170");
	 source->addConfig("y","40");
	 source->addConfig("depth","2.5");
	 source->addConfig("delta_x","4");
	 source->addConfig("delta_y","4");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("finished_when_reached","true");
	 source->addConfig("heading","0.7853981633974483");
	 source->addConfig("timeout","480");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("depth","0.5");
	 target->addConfig("timeout","15");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("x","-170");
	 trigger->addConfig("y","40");
	 trigger->addConfig("depth","2.5");
	 trigger->addConfig("delta_x","4");
	 trigger->addConfig("delta_y","4");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("finished_when_reached","true");
	 trigger->addConfig("heading","0.7853981633974483");
	 trigger->addConfig("timeout","480");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::Anomaly",pool);
	auto start_state = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 start_state->addConfig("timeout","300");
	 start_state->addConfig("corner","1");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 source->addConfig("timeout","300");
	 source->addConfig("corner","1");
	auto target = pool->getComponent("Main::WallContinue");
	auto trigger = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 trigger->addConfig("timeout","300");
	 trigger->addConfig("corner","1");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::WallContinue");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-10");
	 target->addConfig("y","37");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("Main::WallContinue");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-10");
	 source->addConfig("y","37");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-10.5");
	 target->addConfig("y","37.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-10");
	 trigger->addConfig("y","37");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-10.5");
	 source->addConfig("y","37.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-9.5");
	 target->addConfig("y","36.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-10.5");
	 trigger->addConfig("y","37.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-9.5");
	 source->addConfig("y","36.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-14");
	 target->addConfig("y","37");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-9.5");
	 trigger->addConfig("y","36.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-14");
	 source->addConfig("y","37");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-14.5");
	 target->addConfig("y","37.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-14");
	 trigger->addConfig("y","37");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-14.5");
	 source->addConfig("y","37.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-13.5");
	 target->addConfig("y","36.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-14.5");
	 trigger->addConfig("y","37.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-13.5");
	 source->addConfig("y","36.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-22");
	 target->addConfig("y","38");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-13.5");
	 trigger->addConfig("y","36.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-22");
	 source->addConfig("y","38");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-22.5");
	 target->addConfig("y","38.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","38");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-22.5");
	 source->addConfig("y","38.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-21.5");
	 target->addConfig("y","37.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-22.5");
	 trigger->addConfig("y","38.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-21.5");
	 source->addConfig("y","37.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-12");
	 target->addConfig("y","37");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-21.5");
	 trigger->addConfig("y","37.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-12");
	 source->addConfig("y","37");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-12.5");
	 target->addConfig("y","37.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-12");
	 trigger->addConfig("y","37");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-12.5");
	 source->addConfig("y","37.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-11.5");
	 target->addConfig("y","36.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-12.5");
	 trigger->addConfig("y","37.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-11.5");
	 source->addConfig("y","36.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-14");
	 target->addConfig("y","42");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-11.5");
	 trigger->addConfig("y","36.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-14");
	 source->addConfig("y","42");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-14.5");
	 target->addConfig("y","42.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-14");
	 trigger->addConfig("y","42");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-14.5");
	 source->addConfig("y","42.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-13.5");
	 target->addConfig("y","41.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-14.5");
	 trigger->addConfig("y","42.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::WorldXYPositionCmp");}
	try{
	auto sm = new StateMachine("Main::SearchBuoy1Machine",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("delta_timeout","2");
	 start_state->addConfig("x","-10");
	 start_state->addConfig("y","37");
	 start_state->addConfig("depth","1.5");
	 start_state->addConfig("heading","3.141592653589793");
	 start_state->addConfig("timeout","90");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-10");
	 source->addConfig("y","37");
	 source->addConfig("depth","1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-10.5");
	 target->addConfig("y","37.5");
	 target->addConfig("depth","1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-10");
	 trigger->addConfig("y","37");
	 trigger->addConfig("depth","1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-10.5");
	 source->addConfig("y","37.5");
	 source->addConfig("depth","1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-9.5");
	 target->addConfig("y","36.5");
	 target->addConfig("depth","1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-10.5");
	 trigger->addConfig("y","37.5");
	 trigger->addConfig("depth","1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::SearchBuoy2Machine",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("delta_timeout","2");
	 start_state->addConfig("x","-15");
	 start_state->addConfig("y","35");
	 start_state->addConfig("depth","1.5");
	 start_state->addConfig("heading","3.141592653589793");
	 start_state->addConfig("timeout","90");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-15");
	 source->addConfig("y","35");
	 source->addConfig("depth","1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-15.5");
	 target->addConfig("y","35.5");
	 target->addConfig("depth","1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-15");
	 trigger->addConfig("y","35");
	 trigger->addConfig("depth","1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-15.5");
	 source->addConfig("y","35.5");
	 source->addConfig("depth","1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-14.5");
	 target->addConfig("y","34.5");
	 target->addConfig("depth","1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-15.5");
	 trigger->addConfig("y","35.5");
	 trigger->addConfig("depth","1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::SearchBuoy3Machine",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("delta_timeout","2");
	 start_state->addConfig("x","-22");
	 start_state->addConfig("y","38");
	 start_state->addConfig("depth","1.5");
	 start_state->addConfig("heading","3.141592653589793");
	 start_state->addConfig("timeout","90");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-22");
	 source->addConfig("y","38");
	 source->addConfig("depth","1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-22.5");
	 target->addConfig("y","38.5");
	 target->addConfig("depth","1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","38");
	 trigger->addConfig("depth","1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-22.5");
	 source->addConfig("y","38.5");
	 source->addConfig("depth","1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-21.5");
	 target->addConfig("y","37.5");
	 target->addConfig("depth","1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-22.5");
	 trigger->addConfig("y","38.5");
	 trigger->addConfig("depth","1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::SearchBuoy4Machine",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("delta_timeout","2");
	 start_state->addConfig("x","-12");
	 start_state->addConfig("y","37");
	 start_state->addConfig("depth","1.5");
	 start_state->addConfig("heading","1.5707963267948966");
	 start_state->addConfig("timeout","90");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-12");
	 source->addConfig("y","37");
	 source->addConfig("depth","1.5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-12.5");
	 target->addConfig("y","37.5");
	 target->addConfig("depth","1.5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-12");
	 trigger->addConfig("y","37");
	 trigger->addConfig("depth","1.5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-12.5");
	 source->addConfig("y","37.5");
	 source->addConfig("depth","1.5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-11.5");
	 target->addConfig("y","36.5");
	 target->addConfig("depth","1.5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-12.5");
	 trigger->addConfig("y","37.5");
	 trigger->addConfig("depth","1.5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::SearchBuoy5Machine",pool);
	auto start_state = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 start_state->addConfig("finish_when_reached","true");
	 start_state->addConfig("delta_timeout","2");
	 start_state->addConfig("x","-10");
	 start_state->addConfig("y","39");
	 start_state->addConfig("depth","1.5");
	 start_state->addConfig("heading","0");
	 start_state->addConfig("timeout","90");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-10");
	 source->addConfig("y","39");
	 source->addConfig("depth","1.5");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-10.5");
	 target->addConfig("y","39.5");
	 target->addConfig("depth","1.5");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-10");
	 trigger->addConfig("y","39");
	 trigger->addConfig("depth","1.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-10.5");
	 source->addConfig("y","39.5");
	 source->addConfig("depth","1.5");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-9.5");
	 target->addConfig("y","38.5");
	 target->addConfig("depth","1.5");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-10.5");
	 trigger->addConfig("y","39.5");
	 trigger->addConfig("depth","1.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::PositionMoveCmp");}
	try{
	auto sm = new StateMachine("Main::AnomalyIntelly",pool);
	auto start_state = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 start_state->addConfig("depth","-1.5");
	 start_state->addConfig("heading","-3.141592653589793");
	 start_state->addConfig("x_speed","1");
	 start_state->addConfig("y_speed","0");
	 start_state->addConfig("timeout","20");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","-3.141592653589793");
	 source->addConfig("x_speed","1");
	 source->addConfig("y_speed","0");
	 source->addConfig("timeout","20");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("x","-22");
	 target->addConfig("y","44");
	 target->addConfig("depth","2");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","-3.141592653589793");
	 trigger->addConfig("x_speed","1");
	 trigger->addConfig("y_speed","0");
	 trigger->addConfig("timeout","20");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("x","-22");
	 source->addConfig("y","44");
	 source->addConfig("depth","2");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("x","-22");
	 target->addConfig("y","44");
	 target->addConfig("depth","2");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","44");
	 trigger->addConfig("depth","2");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("x","-22");
	 source->addConfig("y","44");
	 source->addConfig("depth","2");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("Main::WallAndBuoy");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","44");
	 trigger->addConfig("depth","2");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::WallAndBuoy");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("timeout","5");
	 target->addConfig("depth","-2");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("x_speed","0");
	 target->addConfig("y_speed","0");
	auto trigger = pool->getComponent("Main::WallAndBuoy");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("timeout","5");
	 source->addConfig("depth","-2");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("x_speed","0");
	 source->addConfig("y_speed","0");
	auto target = pool->getComponent("AuvCont::WorldXYZPositionCmp");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("timeout","5");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("x_speed","0");
	 trigger->addConfig("y_speed","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::WorldXYZPositionCmp");
	auto target = pool->getComponent("Main::WallContinue");
	auto trigger = pool->getComponent("AuvCont::WorldXYZPositionCmp");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::WallContinue");
	auto target = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 target->addConfig("timeout","20");
	auto trigger = pool->getComponent("Main::WallContinue");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 source->addConfig("timeout","20");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("depth","-2");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("heading","-2.356194490192345");
	 target->addConfig("x","-22");
	 target->addConfig("y","25");
	 target->addConfig("timeout","150");
	auto trigger = pool->getComponent("AuvCont::WorldXYPositionCmp")->getSpecialized();
	 trigger->addConfig("timeout","20");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("depth","-2");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("heading","-2.356194490192345");
	 source->addConfig("x","-22");
	 source->addConfig("y","25");
	 source->addConfig("timeout","150");
	auto target = pool->getComponent("Main::SearchBuoy1Machine");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("heading","-2.356194490192345");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","25");
	 trigger->addConfig("timeout","150");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::SearchBuoy1Machine");
	auto target = pool->getComponent("Main::SearchBuoy2Machine");
	auto trigger = pool->getComponent("Main::SearchBuoy1Machine");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::SearchBuoy2Machine");
	auto target = pool->getComponent("Main::SearchBuoy3Machine");
	auto trigger = pool->getComponent("Main::SearchBuoy2Machine");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::SearchBuoy3Machine");
	auto target = pool->getComponent("Main::SearchBuoy4Machine");
	auto trigger = pool->getComponent("Main::SearchBuoy3Machine");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::SearchBuoy4Machine");
	auto target = pool->getComponent("Main::SearchBuoy5Machine");
	auto trigger = pool->getComponent("Main::SearchBuoy4Machine");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvCont::MoveCmp");}
	try{
	auto sm = new StateMachine("Main::Combined",pool);
	auto start_state = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 start_state->addConfig("timeout","20");
	 start_state->addConfig("depth","-2");
	 start_state->addConfig("x_speed","0");
	 start_state->addConfig("y_speed","0");
sm->setStart(start_state);
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("timeout","20");
	 source->addConfig("depth","-2");
	 source->addConfig("x_speed","0");
	 source->addConfig("y_speed","0");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-10");
	 target->addConfig("y","37");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("timeout","20");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("x_speed","0");
	 trigger->addConfig("y_speed","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-10");
	 source->addConfig("y","37");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-10.5");
	 target->addConfig("y","37.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-10");
	 trigger->addConfig("y","37");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-10.5");
	 source->addConfig("y","37.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-9.5");
	 target->addConfig("y","36.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-10.5");
	 trigger->addConfig("y","37.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-9.5");
	 source->addConfig("y","36.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-15");
	 target->addConfig("y","35");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-9.5");
	 trigger->addConfig("y","36.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-15");
	 source->addConfig("y","35");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-15.5");
	 target->addConfig("y","35.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-15");
	 trigger->addConfig("y","35");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-15.5");
	 source->addConfig("y","35.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-14.5");
	 target->addConfig("y","34.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-15.5");
	 trigger->addConfig("y","35.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-14.5");
	 source->addConfig("y","34.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-22");
	 target->addConfig("y","38");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-14.5");
	 trigger->addConfig("y","34.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-22");
	 source->addConfig("y","38");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-22.5");
	 target->addConfig("y","38.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","38");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-22.5");
	 source->addConfig("y","38.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-21.5");
	 target->addConfig("y","37.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","3.141592653589793");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-22.5");
	 trigger->addConfig("y","38.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-21.5");
	 source->addConfig("y","37.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","3.141592653589793");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-12");
	 target->addConfig("y","37");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-21.5");
	 trigger->addConfig("y","37.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","3.141592653589793");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-12");
	 source->addConfig("y","37");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-12.5");
	 target->addConfig("y","37.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-12");
	 trigger->addConfig("y","37");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-12.5");
	 source->addConfig("y","37.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-11.5");
	 target->addConfig("y","36.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-12.5");
	 trigger->addConfig("y","37.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-11.5");
	 source->addConfig("y","36.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-10");
	 target->addConfig("y","39");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-11.5");
	 trigger->addConfig("y","36.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-10");
	 source->addConfig("y","39");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-10.5");
	 target->addConfig("y","39.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-10");
	 trigger->addConfig("y","39");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-10.5");
	 source->addConfig("y","39.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","2");
	 target->addConfig("x","-9.5");
	 target->addConfig("y","38.5");
	 target->addConfig("depth","-1.5");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-10.5");
	 trigger->addConfig("y","39.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","2");
	 source->addConfig("x","-9.5");
	 source->addConfig("y","38.5");
	 source->addConfig("depth","-1.5");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("timeout","20");
	 target->addConfig("depth","-2");
	 target->addConfig("x_speed","0");
	 target->addConfig("y_speed","0");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","2");
	 trigger->addConfig("x","-9.5");
	 trigger->addConfig("y","38.5");
	 trigger->addConfig("depth","-1.5");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("timeout","20");
	 source->addConfig("depth","-2");
	 source->addConfig("x_speed","0");
	 source->addConfig("y_speed","0");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("x","-22");
	 target->addConfig("y","44");
	 target->addConfig("depth","-2");
	 target->addConfig("heading","1.5707963267948966");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("timeout","20");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("x_speed","0");
	 trigger->addConfig("y_speed","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("x","-22");
	 source->addConfig("y","44");
	 source->addConfig("depth","-2");
	 source->addConfig("heading","1.5707963267948966");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 target->addConfig("finish_when_reached","true");
	 target->addConfig("delta_timeout","5");
	 target->addConfig("x","-22");
	 target->addConfig("y","44");
	 target->addConfig("depth","-2");
	 target->addConfig("heading","0");
	 target->addConfig("timeout","90");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","44");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("heading","1.5707963267948966");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 source->addConfig("finish_when_reached","true");
	 source->addConfig("delta_timeout","5");
	 source->addConfig("x","-22");
	 source->addConfig("y","44");
	 source->addConfig("depth","-2");
	 source->addConfig("heading","0");
	 source->addConfig("timeout","90");
	auto target = pool->getComponent("Main::WallAndBuoy");
	auto trigger = pool->getComponent("AuvCont::PositionMoveCmp")->getSpecialized();
	 trigger->addConfig("finish_when_reached","true");
	 trigger->addConfig("delta_timeout","5");
	 trigger->addConfig("x","-22");
	 trigger->addConfig("y","44");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("heading","0");
	 trigger->addConfig("timeout","90");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("Main::WallAndBuoy");
	auto target = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 target->addConfig("timeout","5");
	 target->addConfig("depth","-2");
	 target->addConfig("heading","-1.5707963267948966");
	 target->addConfig("x_speed","0");
	 target->addConfig("y_speed","0");
	auto trigger = pool->getComponent("Main::WallAndBuoy");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 source->addConfig("timeout","5");
	 source->addConfig("depth","-2");
	 source->addConfig("heading","-1.5707963267948966");
	 source->addConfig("x_speed","0");
	 source->addConfig("y_speed","0");
	auto target = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 target->addConfig("timeout","15");
	 target->addConfig("depth","2");
	 target->addConfig("x_speed","0");
	 target->addConfig("y_speed","0");
	auto trigger = pool->getComponent("AuvCont::MoveCmp")->getSpecialized();
	 trigger->addConfig("timeout","5");
	 trigger->addConfig("depth","-2");
	 trigger->addConfig("heading","-1.5707963267948966");
	 trigger->addConfig("x_speed","0");
	 trigger->addConfig("y_speed","0");
	sm->addTransition(source,target,trigger,"success");
}
{
	auto source = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 source->addConfig("timeout","15");
	 source->addConfig("depth","2");
	 source->addConfig("x_speed","0");
	 source->addConfig("y_speed","0");
	auto target = pool->getComponent("AuvControl::MotionControlCmp");
	auto trigger = pool->getComponent("AuvControl::SimpleMove")->getSpecialized();
	 trigger->addConfig("timeout","15");
	 trigger->addConfig("depth","2");
	 trigger->addConfig("x_speed","0");
	 trigger->addConfig("y_speed","0");
	sm->addTransition(source,target,trigger,"success");
}
	}catch(...){printf("cannot (2) get AuvControl::SimpleMove");}
	}

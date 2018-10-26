<?php


require_once './vendor/autoload.php';

use Kreait\Firebase\Factory;
use Kreait\Firebase\ServiceAccount;


class Users
{

protected $database;
protected $dbname ='users';

    public function __construct()
    {
        $acc = ServiceAccount::fromJsonFile(__DIR__. '/secret/proyectoiieunal-bdba809db527.json');
        $firebase = (new Factory)->withServiceAccount($acc)->create();

        $this->database = $firebase->getDatabase();
    }

    public function get(int $userID = Null)
    {
        if(empty($userID) || !isset($userID)){return False;}
        if($this->database->getReference($this->dbname)->getSnapshot()->hasChild($userID)){
            return $this->database->getReference($this->dbname)->getChild($userID)->getValue();
        }
    }

    public function insert(array $data)
    {
        if(empty($data) || !isset($data)){return False;}

        foreach ($data as $key => $value){
            $this->database->getReference()->getChild($this->dbname)->getChild($key)->set($value);
        }


        return TRUE;

    }

    public function delete(int $userID)
    {
        if(empty($userID) || !isset($userID)){return False;}

        if($this->database->getReference($this->dbname)->getSnapshot()->hasChild($userID))
        {
            $this->database->getReference($this->dbname)->getChild($userID)->remove();
            return TRUE;
        }
        else{return FALSE;}
    }

}

$users = new Users();

var_dump($users->insert([
    '1'=>$_POST['valor']
]));

var_dump($users->get(1));

var_dump($users->delete(2));
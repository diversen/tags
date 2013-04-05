### Tags

tags module can be used in other modules with the event system. 
For adding a tag input elements to a 'create' form use: 

    // trigger form events
    event::triggerEvent(
        config::getModuleIni('content_article_events'), 
        array(
            'action' => 'form',
            'reference' => 'gallery'; // unique - e.g. module name
        )
    );

For adding a tag input (notice the parent_id wich has to be a reference to 
to e.g. the parent article) to a 'update' form use: 

    event::triggerEvent(
        config::getModuleIni('content_article_events'), 
        array(
            'action' => 'form',
            'reference' => contentArticle::$reference,
            'parent_id' => $id)
    );

Trigger an event on a insert operation. 

    $event_params = array (
        'action' => 'insert',
        'reference' => self::$reference, 
         'parent_id' => $last_insert_id);
            
    // trigger events
    event::triggerEvent(
        config::getModuleIni('content_article_events'),
        $event_params
    );

Trigger this event when delete something

    $event_params = array (
        'action' => 'delete',
        'reference' => self::$reference, 
        'parent_id' => $id);
            
    event::triggerEvent(
        config::getModuleIni('content_article_crud_events'),
        $event_params
    );

Trigger this event when updating something

    $event_params = array (
        'action' => 'update',
        'reference' => 'gallery', 
        'parent_id' => $id);
            
    event::triggerEvent(
        config::getModuleIni('content_article_crud_events'),
        $event_params
    );  

Displaying tags is done with 'view' which will echo the tags.

    $event_params = array(
        'action' => 'view',
        'reference' => 'gallery',
        'parent_id' => $val['id']);
            
    event::triggerEvent(
        config::getModuleIni('gallery_events'), 
        $event_params
    );      

Currently no method to just return the html.

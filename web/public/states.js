window.haStates = [{"id":"media_player.unnamed_room","name":"Outside","on":false},{"id":"media_player.unnamed_room_2","name":"Dining Room","on":false},{"id":"climate.upstairs","name":"Upstairs","on":false},{"id":"automation.light_on_in_morning","name":"Light on in morning","on":false},{"id":"automation.some_button","name":"Some Button","on":true},{"id":"automation.vacation_on_automation","name":"Vacation ON Automation","on":true},{"id":"automation.vacation_off_automation","name":"Vacation OFF Automation","on":true},{"id":"media_player.living_room_tv","name":"Living Room TV","on":false},{"id":"light.hall_2_light","name":"Hall 2 Light","on":false},{"id":"switch.barbecue_1","name":"Barbecue 1","on":false},{"id":"switch.barbecue_2","name":"Barbecue 2","on":false},{"id":"switch.front_lawn","name":"Front Lawn","on":false},{"id":"light.flood_lights","name":"Flood Lights","on":false},{"id":"switch.outdoor_plug","name":"Outdoor Plug","on":true},{"id":"light.master_bedroom_light","name":"Master Bedroom Light","on":false},{"id":"light.front_tree_light","name":"Front Tree Light","on":false},{"id":"light.hall_1_light","name":"Hall 1 Light","on":false},{"id":"light.back_tree_light","name":"Back Tree Light","on":false},{"id":"light.front_lights","name":"Front Lights","on":false},{"id":"light.chandelier","name":"Chandelier","on":false},{"id":"switch.hvac_enable","name":"Hvac Enable","on":true},{"id":"switch.off_sunset","name":"Off Sunset","on":false},{"id":"switch.valve","name":"Valve","on":false}];

const ids = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 'A', 'B'];
function initialStates() {
    const st = {};
    for (const i of ids) {
        st[i] = haStates[Math.floor(Math.random() * haStates.length)];
    }
    return st;
}
window.states = initialStates();

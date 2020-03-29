// Set devMode if the fake states have been loaded
const devMode = !!window.haStates;

export const ids = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 'A', 'B'];


/** Return all states from home assistant. */
export async function fetchHaStates() {
    if (window.haStates) return window.haStates;
    const res = await fetch('/hastates');
    return await res.json();
}

/** Update the states on the server. */
export async function postStates(states) {
    if (devMode) {
        return await new Promise(resolve => setTimeout(resolve, 200));
    }
    // Remove the "on" property from states to be submitted
    const cleanedStates = {};
    for (const id of ids) {
        if (!states[id]) continue;
        cleanedStates[id] = {
            name: states[id].name,
            id: states[id].id,
        }
    };
    // Make a POST request to the backend server
    const res = await fetch('/states', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(cleanedStates),
    });
    if (res.status >= 400) throw new Error(`Bad status code ${res.status}`);
}

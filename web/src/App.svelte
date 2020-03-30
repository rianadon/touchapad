<script>
 import Dropdown from './Dropdown.svelte';
 import Arrow from './Arrow.svelte';
 import Button from './Button.svelte';
 import TimeAgo from './TimeAgo.svelte';
 import { ids, postStates } from './api.js';

 export let haStates;

 let selIdx = void 0;
 let states = window.states;
 let history = [clone(states)];
 let lastModified = new Date(0);
 let saving = false;
 let width;

// window.states doesn't have on/off info, so add it!
 for (const state of Object.values(states)) {
     const match = haStates.find(e => e.id == state.id);
     if (match && match.on) state.on = true;
 }

 /** Determine if the current state is a unique in history. */
 function newState() {
     if (history.length === 0) return true;
     const histat = history[history.length - 1];
     for (const i of ids) {
         if (typeof states[i] != typeof histat[i]) return true;
         if (!states[i]) continue;
         if (states[i].name != histat[i].name
             || states[i].id != histat[i].id) return true;
     }
     return false;
 }

 /** Return a copy of the given state. */
 function clone(state) {
     let newState = new Object();
     for (const i of ids) {
         newState[i] = state[i];
     }
     return newState;
 }

 function undo() {
     // Make sure the original state is always in history
     // So that the user can revert
     if (history.length > 1) {
         history.pop();
         states = clone(history[history.length-1]);
     }
 }

 function revert() {
     while (history.length > 1) undo();
 }

 $: if (newState()) {
     history = [...history, clone(states)];
     saving = true;
     postStates(states)
         .then(_ => saving = false)
         .catch(e => alert('Could not save!: ' + e) || console.log(e));
     lastModified = new Date();
 }

 // Event listeners for accessibility
 const keyUp = f => e => {
     if (e.keyCode === 32) {
         e.preventDefault();
         f()
     }
 };
 const keyDown = f => e => {
     if (e.keyCode === 32) e.preventDefault();
     if (e.keyCode === 13) {
         e.preventDefault();
         f();
     }
 }

</script>

<svelte:window bind:innerWidth={width}/>
<main>
	<h1>Edit Controls</h1>
    <section class="touchpadsec">
        <div class="touchpad">
            {#each ids as i}
                <div class="touchbutton { states[i] && states[i].on ? 'on' : ''} { selIdx === i ? 'selected' : ''}" on:click={e => selIdx=i} tabIndex={width < 960 ? 0 : null} on:keydown={keyDown(_ => selIdx=i)} on:keyup={keyUp(_ => selIdx=i)}>
                    {states[i] ? states[i].name : ''}
                    <svg width="100" height="40" viewBox="0 0 100 40">
                        <rect x="1.5" y="1.5" width="97" height="37" rx="4.5" fill="none" stroke="#9D9D9D" stroke-width="3" stroke-miterlimit="3" stroke-linecap="round" stroke-linejoin="round" stroke-dasharray="8 13"/>
                    </svg>
                </div>
            {/each}
        </div>
        <svg class="arrows" width="800" height="430" fill="none">
            <Arrow points={[400-240, 16, 0.3, 65, 400-187]} />
            <Arrow points={[400+240, 16, 0.3, 65, 400+187]} />
            <Arrow points={[400-240, 100, 0.3, 117, 400-187]} />
            <Arrow points={[400+240, 100, 0.3, 117, 400+187]} />
            <Arrow points={[400-240, 184, 0.3, 169, 400-187]} />
            <Arrow points={[400+240, 184, 0.3, 169, 400+187]} />
            <Arrow points={[400-240, 268, 0.3, 221, 400-187]} />
            <Arrow points={[400+240, 268, 0.3, 221, 400+187]} />
            <Arrow points={[400-30, 352, 400-24, 312, 400-70, 65, 400-56]} />
            <Arrow points={[400-30, 420, 400-16, 304, 400-62, 117, 400-56]} />
            <Arrow points={[400+30, 352, 400+24, 312, 400+70, 169, 400+56]} />
            <Arrow points={[400+30, 420, 400+16, 304, 400+62, 221, 400+56]} />
        </svg>
        <svg class="arrows arrows-small" width="800" height="430" fill="none">
            <Arrow points={[400-140, 4, 0.6, 42+31, 400-96]} />
            <Arrow points={[400+140, 4, 0.6, 42+31, 400+96]} />
            <Arrow points={[400-140, 76, 0.6, 68+31, 400-96]} />
            <Arrow points={[400+140, 76, 0.6, 68+31, 400+96]} />
            <Arrow points={[400-140, 148, 0.6, 94+31, 400-96]} />
            <Arrow points={[400+140, 148, 0.6, 94+31, 400+96]} />
            <Arrow points={[400-140, 220, 0.6, 120+31, 400-96]} />
            <Arrow points={[400+140, 220, 0.6, 120+31, 400+96]} />
            <Arrow points={[400-30, 352-60, 400-24, 312-60, 400-42, 42+31, 400-28]} />
            <Arrow points={[400-30, 420-60, 400-16, 304-60, 400-37, 68+31, 400-28]} />
            <Arrow points={[400+30, 352-60, 400+24, 312-60, 400+42, 94+31, 400+28]} />
            <Arrow points={[400+30, 420-60, 400+16, 304-60, 400+37, 120+31, 400+28]} />
        </svg>
        <div class="leftdrops">
            {#each [0, 1, 2, 3] as i}
                <div class="dropwrapper">
                    <Dropdown bind:selectedItem={states[i]} items={haStates} />
                </div>
            {/each}
        </div>
        <div class="botdrops">
            {#each [4, 5, 6, 7] as i}
                <div class="dropwrapper">
                    <Dropdown bind:selectedItem={states[i]} items={haStates} />
                </div>
            {/each}
        </div>
        <div class="rightdrops">
            {#each [8, 9, 'A', 'B'] as i}
                <div class="dropwrapper">
                    <Dropdown bind:selectedItem={states[i]} items={haStates} />
                </div>
            {/each}
        </div>
        <div class="singledrop">
            {#if typeof selIdx === 'undefined'}
                Select a button to edit
            {:else}
                <Dropdown bind:selectedItem={states[selIdx]} items={haStates} />
            {/if}
        </div>
    </section>
    <section class="buttonsec">
        <Button on:click={undo}>
            <svg width="24" height="24" viewBox="0 0 24 24">
                <path d="M12.4662 8C9.81617 8 7.41617 8.99 5.56617 10.6L3.67617 8.71C3.04617 8.08 1.96617 8.52 1.96617 9.41V15C1.96617 15.55 2.41617 16 2.96617 16H8.55617C9.44617 16 9.89617 14.92 9.26617 14.29L7.35617 12.38C8.74617 11.22 10.5162 10.5 12.4762 10.5C15.6362 10.5 18.3662 12.34 19.6662 15C19.9362 15.56 20.5762 15.84 21.1662 15.64C21.8762 15.41 22.2362 14.6 21.9162 13.92C20.1962 10.42 16.6162 8 12.4662 8Z" fill="#555" />
            </svg>
            Undo
        </Button>
        <Button on:click={revert}>
            <svg width="24" height="24" viewBox="0 0 24 24">
                <path d="M13.5 7C15.2239 7 16.8772 7.68482 18.0962 8.90381C19.3152 10.1228 20 11.7761 20 13.5C20 15.2239 19.3152 16.8772 18.0962 18.0962C16.8772 19.3152 15.2239 20 13.5 20H11C10.4477 20 10 19.5523 10 19V19C10 18.4477 10.4477 18 11 18H13.5C16 18 18 16 18 13.5C18 11 16 9 13.5 9H7.83L10.2067 11.3844C10.5953 11.7743 10.5948 12.4052 10.2056 12.7944V12.7944C9.81589 13.1841 9.1841 13.1841 8.79443 12.7944L4.70711 8.70711C4.31658 8.31658 4.31658 7.68342 4.70711 7.29289L8.79539 3.20461C9.18494 2.81506 9.81657 2.81434 10.2075 3.20251V3.20251C10.6004 3.59262 10.6019 4.22811 10.2104 4.61961L7.83 7H13.5ZM6 19C6 18.4477 6.44772 18 7 18V18C7.55228 18 8 18.4477 8 19V19C8 19.5523 7.55228 20 7 20V20C6.44772 20 6 19.5523 6 19V19Z" fill="#555555"/>
            </svg>
            Revert
        </Button>
        <div class="lastsave">
            {#if saving}
                Saving changes ...<br>don't blink
            {:else}
                Changes last saved<br><TimeAgo date={lastModified} />
            {/if}
        </div>
    </section>
</main>

<style>
 main {
	 text-align: center;
	 margin: 0 auto;
 }

 .touchpadsec {
     position: relative;
     background-color: #eee;
     padding: 16px;
     height: 440px;
 }

 h1 {
     line-height: 1.1;
	 font-size: 4em;
	 font-weight: 100;
     margin: 0.9em 0;
 }

 .touchpad {
     display: inline-flex;
     flex-direction: column;
     flex-wrap: wrap;
     background-color: #fff;
     border-radius: 6px;
     border: solid 3px #9d9d9d;
     padding: 20px 26px;
     width: 442px;
     height: 254px;
 }

 .touchbutton {
     height: 40px;
     line-height: 40px;
     width: 100px;
     margin: 6px 14px;
     font-size: 12px;
     position: relative;
     overflow: hidden;
     padding: 0 6px;
     border-radius: 6px;
     transition: background-color 0.3s ease-in-out;
 }
 .touchbutton.on {
     background-color: #ddd;
 }

 .touchbutton svg {
     position: absolute;
     left: 0;
     top: 0;
     pointer-events: none;
 }

 .dropwrapper {
     margin: 24px 0;
 }

 .leftdrops {
     position: absolute;
     top: -36px;
     left: calc(50% - 580px);
     width: 330px;
 }

 .rightdrops {
     position: absolute;
     top: -36px;
     right: calc(50% - 580px);
     width: 330px;
 }

 .botdrops {
     position: absolute;
     top: 320px;
     left: calc(50% - 402px);
     width: 804px;
     height: 136px;
     display: flex;
     flex-wrap: wrap;
     flex-direction: column;
 }

 .botdrops .dropwrapper {
     width: 330px;
     margin: 4px 36px;
 }

 .singledrop {
     display: none;
     margin: 32px auto 0 auto;
     max-width: 580px;
 }

 .arrows {
     pointer-events: none;
     position: absolute;
     left: 50%;
     top: 0;
     margin-left: -400px;
 }
 .arrows-small { display: none; }

 .buttonsec {
     margin-top: 48px;
 }

 .lastsave {
     text-align: left;
     font-size: 20px;
     display: inline-block;
     line-height: 30px;
     padding: 10px;
     margin-bottom: 16px;
     width: 220px;
 }

 @media screen and (max-height: 500px) {
     h1 {
         margin: 0.7em 0;
     }
 }

 @media screen and (min-height: 830px) {
     h1 {
         margin: 1em 0;
     }
 }

 @media screen and (max-width: 1176px) {
     .leftdrops { left: 8px; }
     .rightdrops { right: 8px; }
 }

 @media screen and (max-width: 480px), screen and (max-width: 1100px) and (min-width: 977px) {
     .leftdrops { left: calc(50% - 480px); }
     .rightdrops { right: calc(50% - 480px); }
     .botdrops { top: 260px; }
     .touchpad {
         padding: 10px 13px;
         width: 224px;
         height: 130px;
     }
     .touchbutton {
         height: 20px;
         width: 50px;
         margin: 3px 7px;
         color: transparent;
     }
     .touchbutton rect {
         width: 47px;
         height: 17px;
     }
     .arrows { display: none; }
     .arrows-small { display: block; }
     .dropwrapper {
         margin: 12px 0;
     }
     .touchpadsec {
         height: 380px;
         padding-top: 47px;
     }
 }

 @media screen and (max-width: 976px) {
     .leftdrops, .botdrops, .rightdrops, .arrows {
         display: none;
     }
     .touchpad {
         margin-top: -32px;
     }
     .touchpadsec {
         height: 310px;
         margin-bottom: 56px;
     }
     .singledrop {
         display: block;
         font-size: 1.3em;
     }
     .touchbutton { cursor: pointer; }
     .touchbutton.selected {
         background-color: #66CC64;
     }
     .touchbutton.selected rect {
         stroke-dasharray: 0;
         stroke: #555;
     }
 }

 @media screen and (max-width: 480px) and (min-width: 360px) {
     .touchpadsec {
         margin-top: 60px;
         padding-top: 0;
     }
     .touchpad {
         padding: 12px 16px;
         width: 338px;
         height: 190px;
     }
     .touchbutton {
         color: rgba(0, 0, 0, 0.6);
         margin: 4px 10px;
         width: 80px;
         height: 32px;
         line-height: 32px;
         font-size: 9px;
     }
     .touchbutton rect {
         width: 77px;
         height: 29px;
     }
 }

 @media screen and (max-width: 480px) {
     h1 {
         font-size: 3em;
     }
     .touchpadsec {
         height: 240px;
     }
     .lastsave {
         display: block;
         margin-top: 8px;
         text-align: center;
         padding-bottom: 0;
     }
 }

</style>

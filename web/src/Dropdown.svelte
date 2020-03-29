<script>
 import DropdownItem from './DropdownItem.svelte';

 export let items;
 export let selectedItem;
 let guess = '';
 let open = false;
 let flipped = false;
 $: relevantItems = items.filter(i => i.id.toLowerCase().includes(guess.toLowerCase()));
 let bigDropdown, dropdown;

 function getGuess(item) {
     if (!item) return '';
     if (!item.id.includes('.')) return item.id;
     return item.id.substring(0, item.id.indexOf('.')+1);
 }

 function choose(item) {
     return e => {
         e.stopPropagation();
         selectedItem = item;
         open = false;
         dropdown.focus();
     }
 }

 function focus(el) {
     el.focus()
 }

 function openUp() {
     guess = getGuess(selectedItem);
     open = !open;
     const rect = dropdown.getBoundingClientRect();
     const topSpace = rect.top;
     const botSpace = window.innerHeight - rect.bottom;
     console.log(botSpace, topSpace);
     flipped = (botSpace < 300 && topSpace > botSpace);
     console.log(flipped);
 }

 /** Activate the dropdown with the space key. */
 function keyUp(e) {
     if (e.keyCode === 32 && e.target == dropdown) {
         e.preventDefault();
         openUp();
         dropdown.focus();
     }
 }

 /** Activate the dropdown with the enter key */
 function keyDown(e) {
     if (e.keyCode === 32) e.preventDefault();
     if (e.keyCode === 13 && e.target == dropdown) {
         e.preventDefault();
         openUp();
         dropdown.focus();
     }
 }

 /** Use arrow keys to move up and down dropdown items. */
 function upDown(e) {
     const el = document.activeElement;
     if ((e.keyCode == 40 || (flipped && e.keyCode == 38)) && el.tagName === 'INPUT') {
         e.preventDefault();
         el.parentNode.parentNode.querySelector('.choice').focus();
     }
     if (e.keyCode == 40 && el.classList.contains('choice') && el.nextSibling) {
         e.preventDefault();
         el.nextSibling.focus();
     }
     if (e.keyCode == 38 && el.classList.contains('choice')) {
         e.preventDefault();
         if (el.previousSibling) {
             el.previousSibling.focus();
         } else {
             el.parentNode.parentNode.querySelector('input').focus();
         }
     }
     if (e.keyCode == 27) {
         open = false;
     }
 }

 /** Close the dropdown if focus travels off. */
 function blur(e) {
     setTimeout(() => {
         let isThisDropdown = false;
         let el = document.activeElement;
         while (el) {
             if (el == bigDropdown) isThisDropdown = true;
             el = el.parentNode;
         }
         if (!isThisDropdown) open = false;
     }, 1);
 }

</script>

<div class="dropdown" bind:this={bigDropdown} on:keydown={upDown}>
    <div
        aria-haspopup="listbox"
        aria-expanded={open}
        class="line {open ? 'focused' : ''}"
        on:keyup={keyUp} on:keydown={keyDown}
        tabIndex={open ? -1 : 0} bind:this={dropdown}
        on:click={e => openUp()}>
        {#if open}
            <input type="text" bind:value={guess} on:blur={blur} use:focus />
        {:else}
            <DropdownItem item={selectedItem || {}}/>
        {/if}
        <button class="clear" on:click={choose(null)} on:blur={blur}>
            <svg viewBox="0 0 36 36">
                <path d="M27.45 10.565C27.1698 10.2841 26.7893 10.1263 26.3925 10.1263C25.9957 10.1263 25.6153 10.2841 25.335 10.565L18 17.885L10.665 10.55C10.3848 10.2691 10.0043 10.1113 9.60752 10.1113C9.21074 10.1113 8.83026 10.2691 8.55002 10.55C7.96502 11.135 7.96502 12.08 8.55002 12.665L15.885 20L8.55002 27.335C7.96502 27.92 7.96502 28.865 8.55002 29.45C9.13502 30.035 10.08 30.035 10.665 29.45L18 22.115L25.335 29.45C25.92 30.035 26.865 30.035 27.45 29.45C28.035 28.865 28.035 27.92 27.45 27.335L20.115 20L27.45 12.665C28.02 12.095 28.02 11.135 27.45 10.565Z" fill="#9D9D9D"/>
            </svg>
        </button>
        <svg class="down" viewBox="6 0 36 36">
            <path d="M13.0624 17.1206L16.9474 21.0056C17.5324 21.5906 18.4774 21.5906 19.0624 21.0056L22.9474 17.1206C23.8774 16.1756 23.2174 14.5556 21.8824 14.5556L14.1274 14.5556C12.7774 14.5556 12.1174 16.1756 13.0624 17.1206Z" fill="#555555"/>
        </svg>
    </div>
    {#if open}
        <div class="choices {flipped ? 'flipped' : ''}" role="listbox">
            {#each relevantItems as item}
                <button class="choice" tabIndex="0" on:click={choose(item)} on:blur={blur} role="option">
                    <DropdownItem {item} />
                </button>
            {/each}
        </div>
    {/if}
</div>

<style>
 .dropdown {
     position: relative;
     height: 60px;
 }
 .choices {
     width: 100%;
     position: relative;
     z-index: 1;
     background-color: #fff;
     border-radius: 6px;
     border: 2px solid #555;
     margin-top: -2px;
     max-height: 300px;
     overflow: auto;
     box-shadow: 0 2px 0 2px rgba(110,110,110,.5)
 }
 .choices.flipped {
     position: absolute;
     bottom: 58px;
     box-shadow: 0 -2px 0 2px rgba(110,110,110,.5)
 }
 .choice, .choice:hover, .choice:focus {
     display: block;
     width: 100%;
     border-radius: 6px;
     cursor: pointer;
     padding-right: 12px;
     outline: none;
 }
 .choice:first-child { padding-top: 2px; }
 .choice:last-child { padding-bottom: 2px; }
 .choice:hover, .choice:focus { background: #eee; }
 .line {
     background-color: #fff;
     border-radius: 6px;
     display: flex;
     border: solid 2px #555;
     cursor: pointer;
     transition: background-color 0.3s ease-in-out;
 }
 .line:focus, .focused, .clear:focus {
     outline: none;
     box-shadow: 0 0 0 4px rgba(110,110,110,.5)
 }
 .line:hover {
     background-color: #f6f6f6;
 }
 svg {
     width: 30px;
     height: 30px;
     margin-top: 13px;
     margin-bottom: 13px;
     margin-left: 2px;
     margin-right: 4px;
 }
 .clear {
     margin-left: 8px;
 }
 .clear, .down {
     cursor: pointer;
 }
 input {
     color: inherit;
     background: none;
     line-height: 56px;
     padding: 0;
     margin: 0 16px;
     outline: none;
     border: none;
     font-family: monospace;
     font-size: 20px;
     flex-grow: 1;
     width: 0;
 }
 button, button:hover {
     color: inherit;
     background: none;
	 border: none;
	 padding: 0;
     margin: 0;
     line-height: 0;
 }
</style>

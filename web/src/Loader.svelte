<script>
 import { onMount } from 'svelte';
 import { fetchHaStates } from './api.js';
 import App from './App.svelte';

 let haStates;

 onMount(async () => {
	 haStates = await fetchHaStates();
 });
</script>

{#if haStates}
    <App {haStates} />
{:else}
    <div class="loader">
        <div class="spinny"></div>
        <div class="spinnyinny"></div>
    </div>
{/if}

<style>
 .loader {
     position: relative;
     margin: 40vh auto 0 auto;
     border-radius: 6px;
     width: 120px;
     height: 100px;
     background-color: #eee;
     overflow: hidden;
 }
 .spinny {
     position: absolute;
     background-color: #555;
     width: 12px;
     height: 100px;
     left: calc(50% - 6px);
     bottom: 50px;
     transform-origin: 50% 100%;;
     animation: spinny 1.3s ease-in-out infinite;
 }
 .spinnyinny {
     position: absolute;
     top: 12px;
     left: 12px;
     bottom: 12px;
     right: 12px;
     background-color: #fff;
     border-radius: 6px;
 }

 @keyframes spinny {
     50% { width: 80px; }
     100% {transform: rotate(360deg); width: 12px; }
 }
</style>

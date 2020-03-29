<script>
 export let item;
</script>

<div class="item {item.on ? 'on' : ''}">
    {#if (item.id || '').startsWith('light')}
        <svg viewBox="0 0 24 24">
            <path fill="currentColor" d="M12,2A7,7 0 0,0 5,9C5,11.38 6.19,13.47 8,14.74V17A1,1 0 0,0 9,18H15A1,1 0 0,0 16,17V14.74C17.81,13.47 19,11.38 19,9A7,7 0 0,0 12,2M9,21A1,1 0 0,0 10,22H14A1,1 0 0,0 15,21V20H9V21Z" />
        </svg>
    {:else if (item.id || '').startsWith('automation')}
        <svg viewBox="0 0 24 24">
            <path fill="currentColor" d="M12,2A2,2 0 0,1 14,4C14,4.74 13.6,5.39 13,5.73V7H14A7,7 0 0,1 21,14H22A1,1 0 0,1 23,15V18A1,1 0 0,1 22,19H21V20A2,2 0 0,1 19,22H5A2,2 0 0,1 3,20V19H2A1,1 0 0,1 1,18V15A1,1 0 0,1 2,14H3A7,7 0 0,1 10,7H11V5.73C10.4,5.39 10,4.74 10,4A2,2 0 0,1 12,2M7.5,13A2.5,2.5 0 0,0 5,15.5A2.5,2.5 0 0,0 7.5,18A2.5,2.5 0 0,0 10,15.5A2.5,2.5 0 0,0 7.5,13M16.5,13A2.5,2.5 0 0,0 14,15.5A2.5,2.5 0 0,0 16.5,18A2.5,2.5 0 0,0 19,15.5A2.5,2.5 0 0,0 16.5,13Z" />
        </svg>
    {:else if (item.id || '').startsWith('media_player')}
        <svg viewBox="0 0 24 24">
            <path d="M3 3H21C22.1 3 23 3.89999 23 5V19C23 20.1 22.1 21 21 21H15C14.45 21 14 20.55 14 20C14 19.45 14.45 19 15 19H20C20.55 19 21 18.55 21 18V6C21 5.45001 20.55 5 20 5H4C3.45 5 3 5.45001 3 6V7C3 7.54999 2.55 8 2 8C1.45 8 1 7.54999 1 7V5C1 3.89999 1.89999 3 3 3ZM1 15.09C1 14.48 1.53999 13.99 2.14 14.09C5.09 14.57 7.42999 16.9 7.91 19.86C8 20.46 7.52 21 6.91 21C6.42 21 6.00999 20.64 5.92999 20.15C5.57001 18.07 3.92999 16.43 1.85001 16.07C1.36 15.99 1 15.58 1 15.09ZM1 18V21H4C4 19.34 2.66 18 1 18ZM1 11.05C1 10.46 1.50999 10 2.10001 10.05C7.28999 10.57 11.42 14.69 11.95 19.88C12.01 20.47 11.55 20.99 10.95 20.99C10.44 20.99 10.01 20.62 9.96001 20.12C9.55 15.85 6.14999 12.45 1.88 12.04C1.38 11.99 1 11.56 1 11.05Z" fill="currentColor" />
        </svg>
    {:else if (item.id || '').startsWith('climate')}
        <svg viewBox="0 0 24 24">
            <path d="M12 2C13.66 2 15 3.34 15 5V13C16.21 13.91 17 15.37 17 17C17 19.76 14.76 22 12 22C9.23999 22 7 19.76 7 17C7 15.37 7.79001 13.91 9 13V5C9 3.34 10.34 2 12 2ZM11 11H13V9H12.5C12.22 9 12 8.78 12 8.5C12 8.22 12.22 8 12.5 8H13V6H12.5C12.22 6 12 5.78 12 5.5C12 5.22 12.22 5 12.5 5H13C13 4.45 12.55 4 12 4C11.45 4 11 4.45 11 5V11Z" fill="currentColor" />
        </svg>
    {:else if (item.id || '').startsWith('switch')}
        <svg viewBox="0 0 36 36">
            <path d="M11 5.5V19C11 19.825 11.675 20.5 12.5 20.5H15.5V31.225C15.5 31.99 16.505 32.26 16.895 31.6L24.68 18.25C25.265 17.245 24.545 16 23.39 16H20L23.735 6.025C24.11 5.05 23.39 4 22.34 4H12.5C11.675 4 11 4.675 11 5.5Z" fill="currentColor" />
        </svg>
    {:else}
        <svg viewBox="0 0 24 24">
            <path d="M7 5H9V3H7V5ZM7 13H9V11H7V13ZM9 21H7V19H9V21ZM11 17H13V15H11V17ZM13 21H11V19H13V21ZM3 21H5V19H3V21ZM5 17H3V15H5V17ZM3 13H5V11H3V13ZM5 9H3V7H5V9ZM3 5H5V3H3V5ZM13 13H11V11H13V13ZM19 17H21V15H19V17ZM21 13H19V11H21V13ZM19 21H21V19H19V21ZM21 9H19V7H21V9ZM11 9H13V7H11V9ZM19 5V3H21V5H19ZM11 5H13V3H11V5ZM17 21H15V19H17V21ZM15 13H17V11H15V13ZM17 5H15V3H17V5Z" fill="#555"/>
        </svg>
    {/if}
    <div class="inner">
        <div class="name">{item.name || 'Custom Action'}</div>
        <div class="id">{item.id || '<no id>'}</div>
    </div>
</div>

<style>
 .item {
     height: 56px;
     padding: 4px;
     padding-right: 0;
     text-align: left;
     display: flex;
     flex-grow: 1;
 }
 .inner {
     flex-grow: 1;
     overflow: hidden;
     white-space: nowrap;
     width: 0;
 }
 svg {
     margin: 10px;
     margin-right: 12px;
     width: 30px;
     height: 30px;
     color: #555;
 }
 .on svg {
     color: #66CC64;
 }
 .name {
     font-size: 20px;
     line-height: 28px;
 }
 .id {
     font-family: monospace;
     font-size: 14px;
     line-height: 16px;
 }
</style>

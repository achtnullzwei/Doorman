---
layout: doc
footer: false
aside: false
sidebar: false
editLink: false
lastUpdated: false
---

<script setup lang="ts">
import type { DefaultTheme } from 'vitepress/theme'
import { VPButton } from 'vitepress/theme'

import MingcuteTimeFill from '~icons/mingcute/time-fill';
import IonMail from '~icons/ion/mail';
import IcBaselineDiscord from '~icons/ic/baseline-discord';
import PepiconsPopHash from '~icons/pepicons-pop/hash';
</script>

<script lang="ts">
import axios from 'axios'
import moment from 'moment';

const api = axios.create({
   withCredentials: true,
   baseURL: 'https://order.doorman.azon.ai'
})

export default {
    data() {
        return {
            user: null,
            username: '',
            password: '',
            
            stock_add_amount: 0,

            product_data: null,

            orders: [],

            showAll: false,
            orderFilter: '',

            currentPage: 1,
            ordersPerPage: 10,

            modalOpen: false,
            result_title: '',
            result_text: '',
        }
    },
    created() {
        api.get('/user', { 
            withCredentials: true 
        })
        .then(response => {
            this.user = response.data;

            if(this.user != null) {
                this.updateAvailability();
                this.loadOrders();
            }
        })
    },
    computed: {
        filteredOrders() {
            return this.orders.filter(x => {
                if(!this.orderFilter && this.showAll) return true;
                if(!this.orderFilter) return x.status != 'closed' && x.status != 'cancelled';

                return x.name.toLowerCase().includes(this.orderFilter.toLowerCase()) || 
                    x.hash.toLowerCase() == this.orderFilter.toLowerCase().trim() || 
                    x.message.toLowerCase().includes(this.orderFilter.toLowerCase()) ||
                    x.city.toLowerCase().includes(this.orderFilter.toLowerCase()) ||
                    x.street.toLowerCase().includes(this.orderFilter.toLowerCase()) ||
                    x.address_extra.toLowerCase().includes(this.orderFilter.toLowerCase());
            }).sort((a, b) => {
                // Put pending_review first
                if (a.status === 'pending_review' && b.status !== 'pending_review') return -1;
                if (a.status !== 'pending_review' && b.status === 'pending_review') return 1;
                // Then sort by id descending
                return b.id - a.id;
            });
        },
        paginatedOrders() {
            const start = (this.currentPage - 1) * this.ordersPerPage;
            const end = start + this.ordersPerPage;
            return this.filteredOrders.slice(start, end);
        },
        totalPages() {
            return Math.ceil(this.filteredOrders.length / this.ordersPerPage) || 1;
        },
        openOrderCount() {
            return this.orders.filter(x => { return x.status == 'pending_review'; }).length;
        },
        orderTotal() {
            return (order) => {
                const productPrice = (order.product_details ? order.product_details.price : 0) * order.amount;
                const shippingPrice = order.shipping_details ? order.shipping_details.price : 0;
                const price = productPrice + shippingPrice;

                return price;
            }
        },
        statusLabel(){
            return (status) => {
                if(status == 'pending_review') {
                    return 'New';
                } else if(status == 'pending_payment') {
                    return 'Pending payment';
                } else if(status == 'pending_shipment') {
                    return 'Pending shipment';
                } else if(status == 'shipped') {
                    return 'Waiting for arrival';
                } else if(status == 'closed') {
                    return 'Closed';
                } else if(status == 'cancelled') {
                    return 'Cancelled';
                } else {
                    return 'Unknown';
                }
            };
        },
        statusLabelColor(){
            return (status) => {
                if(status == 'pending_review') {
                    return 'warning';
                } else if(status == 'pending_payment') {
                    return 'warning';
                } else if(status == 'pending_shipment') {
                    return 'danger';
                } else if(status == 'shipped') {
                    return 'danger';
                } else if(status == 'closed') {
                    return 'tip';
                } else if(status == 'cancelled') {
                    return 'info';
                } else {
                    return 'info';
                }
            };
        },
        nextStepButtonText(){
            return (status) => {
                if(status == 'pending_review') {
                    return 'Accept';
                } else if(status == 'pending_payment') {
                    return 'Paid';
                } else if(status == 'pending_shipment') {
                    return 'Shipped';
                } else if(status == 'shipped') {
                    return 'Received';
                } else if(status == 'closed') {
                    return '';
                } else if(status == 'cancelled') {
                    return '';
                } else {
                    return '';
                }
            };
        },
    },
    watch: {
        orderFilter() {
            this.currentPage = 1;
        },
        showAll() {
            this.currentPage = 1;
        }
    },
    methods: {
        showModal(title, text) {
            this.modalOpen = true;
            this.result_title = title;
            this.result_text = text;
        },
        async updateUser(login = false) {
            api.get('/user', { 
                withCredentials: true 
            })
            .then(response => {
                this.user = response.data;

                if(this.user != null && login) {
                    this.showModal("Welcome!", "Welcome " + this.user.username + ".");
                }
            });
        },
        async updateAvailability() {
            api.get('/product_data', { 
                withCredentials: true 
            })
            .then(response => {
                this.product_data = response.data;
            });
        },
        async loadOrders() {
            api.get('/orders', { 
                withCredentials: true 
            })
            .then(response => {
                this.orders = response.data;
            })
            .catch(error => {
                this.showModal("Oops!", this.getErrorMessage(error, 'Failed to load orders!'));
            });
        },
        async login() {
            api.post('/login', { username: this.username, password: this.password }, { 
                withCredentials: true 
            })
            .then(response => {
                if(response.data.success) {
                    this.updateUser(true);
                    this.updateAvailability();
                    this.loadOrders();
                } else {
                    this.showModal("Oops!", "Unknown error while logging in!");
                }
            })
            .catch(error => {
                this.showModal("Oops!", this.getErrorMessage(error, 'Failed to login!'));
            });
        },
        async logout() {
            api.post('/logout', { 
                withCredentials: true 
            })
            .then(response => {
                this.updateUser();
                this.showModal("Bye!", "You successfully logged out.");
            })
            .catch(error => {
                this.showModal("Oops!", this.getErrorMessage(error, 'Failed to logout!'));
            });
        },
        async addStock() {
            api.post('/add_stock', { product: 'doorman', amount: this.stock_add_amount }, { 
                withCredentials: true 
            })
            .then(response => {
                this.updateAvailability();
                this.showModal("Updated!", "Stock updated successfully!");
            })
            .catch(error => {
                this.showModal("Oops!", this.getErrorMessage(error, 'Failed to update stock!'));
            });
        },
        async updateOrderStatus(order) {
            if(order.shipping_method == 'tracking' && order.status == 'pending_review') {
                const trackingUrl = prompt('Provide the Tracking ID:');
                if (!trackingUrl) {
                    alert('No Tracking provided! Aborted.');
                    return;
                }
                this.nextStep(order.id, trackingUrl);
            } else {
                this.nextStep(order.id, '');
            }
        },
        async nextStep(id, tracking) {
            api.post('/orders/' + id + '/next', { tracking }, { 
                withCredentials: true 
            })
            .then(response => {
                this.updateAvailability();
                this.loadOrders();
                this.showModal("Updated!", "The order status was updated successfully.");
            })
            .catch(error => {
                this.showModal("Oops!", this.getErrorMessage(error, 'Failed to update order status!'));
            });
        },
        async cancelOrder(id) {
            if (!confirm('Are you sure you want to cancel the order?')) return;

            api.post('/orders/' + id + '/cancel', { 
                withCredentials: true 
            })
            .then(response => {
                this.updateAvailability();
                this.loadOrders();
                this.showModal("Cancelled!", "The order was cancelled successfully!");
            })
            .catch(error => {
                this.showModal("Oops!", this.getErrorMessage(error, 'Failed to cancel order!'));
            });
        },
        async deleteOrder(id) {
            if (!confirm('Are you sure you want to delete the order?')) return;

            api.post('/orders/' + id + '/delete', { 
                withCredentials: true 
            })
            .then(response => {
                this.updateAvailability();
                this.loadOrders();
                this.showModal("Deleted!", "The order was deleted successfully!");
            })
            .catch(error => {
                this.showModal("Oops!", this.getErrorMessage(error, 'Failed to delete order!'));
            });
        },
        getErrorMessage(error, defaultMessage = "Something went wrong.") {
            if (!error) return defaultMessage;

            if (error.response) {
                return error.response.data?.error || error.response.data?.message || `Server error (${error.response.status})`;
            } else if (error.request) {
                return "No response from server. Please check your connection.";
            } else {
                return error.message || defaultMessage;
            }
        }
    }
}
</script>

<style>
.VPDoc .content {
    max-width: 1920px !important;
}
</style>

<style scoped>
textarea,input[type=text],input[type=email],input[type=number],input[type=password],select {
    background-color: var(--vp-c-bg);
    width: 100%;
    font-size: 14px;
    border-radius: 8px;
    border: 1px solid var(--vp-c-divider);
    padding: 10px 12px;
    transition: color .25s,border-color .25s,background-color .25s;
    height: 40px;
    font-family: inherit;
}

textarea:hover,input[type=text]:hover,input[type=email]:hover,input[type=number]:hover,input[type=password]:hover,select:hover {
    border: 1px solid var(--vp-button-brand-active-bg);
    transition: color .1s,border-color .1s,background-color .1s
}

.order_list {
    display: flex;
    flex-direction: column;
    gap: 15px;
}

.order_row {
    display: flex;
    flex-direction: column;
    padding: 15px;
    border-radius: 12px;
    background-color: var(--vp-c-bg-soft);
    color: var(--vp-c-text-2);
}

.order_row .header {
    display: flex;
    gap: 15px;
    align-items: center;
    flex-wrap: wrap; /* erlaubt Umbruch */
    margin-bottom: 15px;
}

.order_row .header .title {
    flex: 1 1 auto; /* Titel nimmt verfügbaren Platz */
    display: flex;
    flex-direction: column;
    width: 60%;
}

.order_row .header .title .VPBadge {
    font-size: 11px;
    padding: 0 5px;
    line-height: 17px;
}

.order_row .header .title .name {
    line-height: 24px;
    font-weight: 600;
    font-size: var(--vp-custom-block-font-size);
}

.order_row .header .title .meta {
    display: flex;
    gap: 0 7px;
    font-size: var(--vp-custom-block-font-size);
    color: var(--vp-c-text-3);
    flex-wrap: wrap;
    align-items: center;
}

.order_row .header .title .meta span {
    display: flex;
    align-items: center;
    gap: 3px;
}

.order_row .header .title .meta span svg {
    display: inline;
}

/* ACTIONS CONTAINER */
.order_row .header .actions {
    display: flex;
    gap: 10px;
    flex: 1 1 50px; /* Basis 250px, darf wachsen */
    min-width: 200px; /* minimum 250px */
}

/* BUTTONS */
.order_row .header .actions button {
    flex: 1 1 0; /* Buttons wachsen gleichmäßig */
    min-width: 80px; /* optional: Mindestbreite */
}

.order_row .details {
    display: flex;
    gap: 15px;
    flex-wrap: wrap;
}

.order_row .details .section {
    flex: 1 1 220px;
    background-color: var(--vp-custom-block-info-bg);
    padding: 12px;
    border-radius: 10px;
    white-space: pre-wrap;
}

.order_row .details b {
    font-size: 14px;
    color: var(--vp-c-text-1);
    margin-bottom: 3px;
    line-height: 14px;
}

.order_row .details .address {
    display: flex;
    flex-direction: column;
    gap: 2px;
    font-size: 14px;
    color: var(--vp-c-text-2);
}

.order_row .details .address span {
    line-height: 17px;
}

.order_summary {
    display: flex;
    flex-direction: column;
    gap: 2px;
    font-size: 14px;
    color: var(--vp-c-text-2);
    margin-top: 4px;
}

.order_summary .item_row {
    display: flex;
    justify-content: space-between;
    align-items: center;
    line-height: 17px;
}

.order_summary .price {
    text-align: right;
    min-width: 70px;
    font-weight: 500;
    color: var(--vp-c-text-1);
}

.order_summary .total {
    margin-top: 6px;
    padding-top: 6px;
    border-top: 1px solid var(--vp-c-gray-3);
}
</style>

<ContactModal :show="modalOpen" @close="modalOpen = false">
    <template #header>
        <h3>{{ result_title }}</h3>
    </template>
    <template #body>
        {{ result_text }}
    </template>
</ContactModal>

<div style="display: flex;justify-content: space-between;align-items:center;">
    <h1 class="firmware_title_row" style="margin-bottom: 10px;margin-top: 10px;">Order Management</h1>
    <VPButton v-if="user" theme="alt" type="button" text="Logout" @click="logout" />
</div>
<div style="margin-bottom: 25px;">
    <span v-if="user">There are currently <b>{{ openOrderCount }}</b> open {{ openOrderCount == 1 ? 'order' : 'orders' }} and <b>{{ product_data?.available_units }}</b> {{ product_data?.available_units == 1 ? 'item' : 'items' }} available.</span>
    <span v-else>Welcome to the Order Management Service! Please login to manage the orders.</span>
</div>
<hr>

<div v-if="!user">
    <h5 class="firmware_title_row" style="margin-bottom: 10px;margin-top: 10px;">Login</h5>
    <div style="display: flex; gap: 15px;justify-content: space-between;align-items:center;">
        <input type="text" name="username" id="username" v-model="username" placeholder="Username" />
        <input type="password" name="password" id="password" v-model="password" placeholder="Password" />
        <VPButton type="button" text="Login" @click="login" />
    </div>
</div>
<div v-else>
    <div style="
        margin-bottom: 25px;
        margin-top: 25px;
        display: flex;
        justify-content: space-between;
        align-items: center;
        flex-wrap: wrap;
        gap: 15px 0;
    ">
        <div style="flex: 1 0 60%; min-width: 250px;">
            <h5 class="firmware_title_row" style="margin-bottom: 5px;margin-top: 0;">Update Stock</h5>
            <div>Add more items and notify customers.</div>
        </div>
        <div style="
            flex: 0 0 40%;
            min-width: 250px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            gap: 15px;
            flex-grow: 1;
        ">
            <input type="number" id="stock_add_amount" v-model="stock_add_amount" min="1">
            <VPButton type="button" text="Add" :disabled="stock_add_amount == 0" @click="addStock" />
        </div>
    </div>
    <hr>
    <div style="
        margin-bottom: 25px;
        margin-top: 25px;
        display: flex;
        justify-content: space-between;
        align-items: center;
        flex-wrap: wrap;
        gap: 15px 0;
    ">
        <div style="flex: 1 0 60%; min-width: 250px;">
            <h5 class="firmware_title_row" style="margin-bottom: 5px;margin-top: 0px;">Orders</h5>
            <div v-if="orderFilter">Filter: Custom (<b>{{ filteredOrders.length }}</b> orders)</div>
            <div v-else-if="showAll">Filter: All orders  (<b>{{ filteredOrders.length }}</b> orders)</div>
            <div v-else>Filter: All ongoing orders  (<b>{{ filteredOrders.length }}</b> orders)</div>
        </div>
        <div style="
            flex: 0 0 40%;
            min-width: 250px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            gap: 15px;
            flex-grow: 1;
        ">
            <input type="text" name="orderFilter" id="orderFilter" placeholder="Search" v-model="orderFilter" />
            <VPButton :theme="showAll ? 'alt' : 'brand'" type="button" :text="showAll ? 'All' : 'Ongoing'" @click="showAll = !showAll" />
        </div>
    </div>
    <div v-if="orders.length > 0" class="order_list">
        <div v-if="orders.length > 0 && filteredOrders.length == 0" class="danger custom-block">
            <p class="custom-block-title">SORRY</p>
            <p>No order matches your filter!</p>
        </div>
        <div v-for="order in paginatedOrders" :key="order.id" class="order_row">
            <div class="header">
                <div class="title">
                    <div class="name">ORDER FROM {{ order.name.split(' ')[0].toUpperCase() }} <Badge :type="statusLabelColor(order.status)">{{ statusLabel(order.status) }}</Badge></div>
                    <div class="meta">
                        <span><MingcuteTimeFill /> {{ moment.unix(order.timestamp).format('DD.MM.YYYY') }}</span>
                        <span><IonMail /> {{ order.email || 'Not available' }}</span>
                        <span v-if="order.discord"><IcBaselineDiscord /> {{ order.discord }}</span>
                        <span><PepiconsPopHash  /> {{ order.hash }}</span>
                    </div>
                </div>
                <div class="actions">
                    <VPButton v-if="nextStepButtonText(order.status)" type="button" :text="nextStepButtonText(order.status)" @click="updateOrderStatus(order)" />
                    <VPButton v-if="order.status != 'closed' && order.status != 'cancelled' && order.status != 'shipped'" theme="alt" type="button" text="Cancel" @click="cancelOrder(order.id)" />
                    <VPButton v-if="order.status == 'cancelled'" theme="alt" type="button" text="Delete" @click="deleteOrder(order.id)" />
                </div>
            </div>
            <div class="details">
                <div class="section">
                    <b>Summary</b>
                    <div class="order_summary">
                        <div class="item_row">
                            <span>{{ order.amount }} × {{ order.product_details.name }}</span>
                            <span class="price">{{ (order.product_details.price * order.amount).toFixed(2) }} €</span>
                        </div>
                        <div class="item_row">
                            <span>1 × {{ order.shipping_details.name }} ({{ order.shipping_region }})</span>
                            <span class="price">{{ order.shipping_details.price.toFixed(2) }} €</span>
                        </div>
                        <div class="item_row total">
                            <span><b>Total via {{ order.payment_details.name }}</b></span>
                            <span class="price"><b>{{ orderTotal(order).toFixed(2) }} €</b></span>
                        </div>
                    </div>
                </div>
                <div class="section">
                    <b>Address in {{ order.country_details.name }}</b>
                    <div class="address">
                        <span>{{ order.name }}</span>
                        <span v-if="order.address_extra">{{ order.address_extra }}</span>
                        <span>{{ order.street }}</span>
                        <span>{{ order.zip }} {{ order.city }}</span>
                    </div>
                </div>
                <div v-if="order.message" class="section">
                    <b>Notes</b>
                    <div class="address">
                        {{ order.message }}
                    </div>
                </div>
            </div>
        </div>
        <div style="display: flex; justify-content: space-between; align-items: center; gap: 10px; margin-top: 20px;">
            <div style="display: flex; align-items: center; gap: 10px;">
                Show:
                <select style="width: 70px;" name="ordersPerPage" class="form-control" id="ordersPerPage" v-model="ordersPerPage">
                    <option value="10">10</option>
                    <option value="20">20</option>
                    <option value="50">50</option>
                    <option value="100">100</option>
                </select>
            </div>
            <div style="display: flex; align-items: center; gap: 10px;">
                <VPButton 
                    theme="alt" 
                    type="button" 
                    text="‹" 
                    :disabled="currentPage === 1"
                    @click="currentPage--"
                />
                <span>Page {{ currentPage }} of {{ totalPages }}</span>
                <VPButton 
                    theme="alt" 
                    type="button" 
                    text="›" 
                    :disabled="currentPage === totalPages"
                    @click="currentPage++"
                />
            </div>
        </div>
    </div>
    <div v-else class="info custom-block">
        <p class="custom-block-title">SORRY</p>
        <p>No order available!</p>
    </div>
</div>
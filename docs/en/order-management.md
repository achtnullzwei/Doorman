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
import PhCpuBold from '~icons/ph/cpu-bold';

import QRCode from 'qrcode';

import {
  Utils,
  RequestCommandId,
  ResponseCommandId,
  NiimbotBluetoothClient,
  ImageEncoder,
  NiimbotSerialClient,
} from "@mmote/niimbluelib";
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

            settingsOpen: false,

            printerClient: null,
            printerClientIsConnecting: false,
            printerClientIsConnected: false,
            printerDebug: false
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
        });
    },
    computed: {
        filteredOrders() {
            return this.orders.filter(x => {
                if(!this.orderFilter && this.showAll) return true;
                if(!this.orderFilter) return x.status != 'closed' && x.status != 'cancelled';

                return x.name.toLowerCase().includes(this.orderFilter.toLowerCase()) || 
                    x.hash.toLowerCase() == this.orderFilter.toLowerCase().trim() || 
                    x.message.toLowerCase().includes(this.orderFilter.toLowerCase()) ||
                    x.discord.toLowerCase().includes(this.orderFilter.toLowerCase()) ||
                    x.model.toLowerCase().includes(this.orderFilter.toLowerCase()) ||
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
            if (!confirm('Are you sure you want update the order status?')) return;

            if(order.status == 'pending_payment') {
                if (!this.printerClientIsConnected) {
                    this.showModal('Warning!', 'Please connect the label printer first!');
                    return;
                }

                try {
                    if(order.shipping_method == 'standard' && order.shipping_region == 'DE') {
                        await this.generateLabel(order, 'standard_sender');
                        await this.print();

                        await this.generateLabel(order, 'standard_receiver');
                        await this.print();
                    } else {
                        await this.generateLabel(order, 'quickstart');
                        await this.print();
                    }
                } catch (e) {
                    console.log(e);
                    this.showModal('Print Error!', e.message);
                    return;
                }
            }

            if(order.shipping_method == 'tracking' && order.status == 'pending_shipment') {
                const trackingUrl = prompt('Provide the Tracking ID:');
                if (!trackingUrl) {
                    this.showModal('Aborted!', 'No tracking provided!');
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
        },
        disconnect() {
            this.printerClient.disconnect();
            this.printerClient = null;

            this.printerClientIsConnecting = false;
            this.printerClientIsConnected = false;
        },
        async connectBLE() {
            this.setupPrinterClient("ble");
            try {
                this.printerClientIsConnecting = true;
                await this.printerClient.connect();
            } catch (e) {
                console.log(e);
                this.showModal('Connection Error!', e.message);
                this.printerClientIsConnecting = false;
            }
        },
        async connectSerial() {
            this.setupPrinterClient("serial");
            try {
                this.printerClientIsConnecting = true;
                await this.printerClient.connect();
            } catch (e) {
                console.log(e);
                this.showModal('Connection Error!', e.message);
                this.printerClientIsConnecting = false;
            }
        },
        async print() {
            if(!this.printerClientIsConnected) {
                this.showModal('Warning!', 'Please connect the label printer first!');
                return;
            }
            
            const printDirection = "top";
            const quantity = 1;

            const canvasEl = this.$refs.labelCanvas;
            if (!canvasEl) {
                this.showModal('Error!', 'Canvas not available for printing!');
                return;
            }
            const encoded = ImageEncoder.encodeCanvas(canvasEl, printDirection);

            const printTaskName = this.printerClient.getPrintTaskType() ?? "B1";

            const printTask = this.printerClient.abstraction.newPrintTask(printTaskName, {
                totalPages: quantity,
                statusPollIntervalMs: 100,
                statusTimeoutMs: 8_000,
                labelType: 1 // WithGaps
            });

            try {
                await printTask.printInit();
                await printTask.printPage(encoded, quantity);
                await printTask.waitForFinished();
            } catch (e) {
                console.log(e);
                this.showModal('Print Error!', e.message);
            } finally {
                await printTask.printEnd();
            }
        },
        setupPrinterClient(transport) {
            if (this.printerClient) {
                this.printerClient.disconnect();
            }

            if (transport === "ble") {
                this.printerClient = new NiimbotBluetoothClient();
            } else if (transport === "serial") {
                this.printerClient = new NiimbotSerialClient();
            }

            this.printerClient.on("packetsent", (e) => {
                console.log(`>> ${Utils.bufToHex(e.packet.toBytes())} (${RequestCommandId[e.packet.command]})`);
            });

            this.printerClient.on("packetreceived", (e) => {
                console.log(`<< ${Utils.bufToHex(e.packet.toBytes())} (${ResponseCommandId[e.packet.command]})`);
            });

            this.printerClient.on("connect", (r) => {
                console.log("connected");
            });

            this.printerClient.on("heartbeatfailed", (res) => {
                if(res.failedAttempts >= 3) {
                    if(!this.printerClientIsConnected) {
                        this.showModal('Oops!', 'Connection to label printer could not be established!');
                    }
                    if (this.printerClient) {
                        this.printerClient.disconnect();
                    }
                }
            });

            this.printerClient.on("packetreceived", (res) => {
                if(!this.printerClientIsConnected) {
                    this.showModal('Connected!', 'Connection to label printer established!');
                    this.printerClientIsConnecting = false;
                    this.printerClientIsConnected = true;
                }
            });

            this.printerClient.on("heartbeat", (res) => {
                if(!this.printerClientIsConnected) {
                    this.showModal('Connected!', 'Connection to label printer established!');
                    this.printerClientIsConnecting = false;
                    this.printerClientIsConnected = true;
                }
            });

            this.printerClient.on("disconnect", () => {
                console.log("disconnected");
                this.printerClientIsConnecting = false;
                this.printerClientIsConnected = false;
            });

            this.printerClient.on("printprogress", (e) => {
                console.log(`Page ${e.page}/${e.pagesTotal}, Page print ${e.pagePrintProgress}%, Page feed ${e.pageFeedProgress}%`);
            });
        },
        async generateLabel(order, type) {
            const canvas = this.$refs.labelCanvas;

            if (!canvas) {
                console.warn("repaint: canvas ref not found");
                return;
            }

            const ctx = canvas.getContext("2d");
            ctx.fillStyle = "white";
            ctx.fillRect(0, 0, canvas.width, canvas.height);

            ctx.fillStyle = "black";
            ctx.textBaseline = "top";

            let x = 5;
            let y = 10;
            const lineHeight = 40;
            const maxWidth = canvas.width - x * 2;
            const bigFont = "bold 32px Inter";
            const normalFont = "32px Inter";
            const smallFont = "28px Inter";

            const loadImage = (src) =>
                new Promise((resolve, reject) => {
                    const img = new Image();
                    img.onload = () => resolve(img);
                    img.onerror = reject;
                    img.src = src;
                });

            const drawQRCodeAndLogo = async (hash) => {
                const size = 90;
                const qrUrl = await QRCode.toDataURL(
                    `https://order.doorman.azon.ai/order/${hash}`,
                    { margin: 1, width: size }
                );

                try {
                    const [qrImg, logoImg] = await Promise.all([
                        loadImage(qrUrl),
                        loadImage(
                            "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAU0AAAFNCAMAAACzLs32AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAGUExURQAAAAAAAKVnuc8AAAACdFJOU/8A5bcwSgAAAAlwSFlzAAAOwwAADsMBx2+oZAAACyVJREFUeF7tltGS4zgOBOf+/6cv1u32qNKSiQJByhOBfEMLrILzYXb//K+p4w//0EzQNiv5V23+ecC/3s3XHTTgxyLh1l18zSEB6FDg8i18xxURqO8dvtjPF5wQgubO4avd3H5ACFq7hi/3cnN9CBr7DF/v5NbyELQ1hgn7uLE6BlVFYMY27msOQU9RmLOJu3pjUFIcJu3hptoYVOTArC3c0xqDgjyYtoNbSmNQjwvzNnBHZwzK8WHiem6ojEE1GZi5nP2NMSgmB1NXs70wCL3kYOpqthfGoJYszF3M7r4YlJKHyWvZXBeETiZg9FL2tgWhkRmYvZS9bTEoZA6mr2RrWRD6mIPpK9laFoM6ZmH+QnZ2BaGNWZi/kJ1dMShjHjasY2NVELqYhw3r2FgVhC7mYcM6NlbFoIoK2LGMfU1BaKICdixjX1MQmqiAHcvY1xSDImpgyyq2FQWhhxrYsoptRUHooQa2rGJbURB6qIEtq9hWFIQeimDNInb1BKGFKtiziF09QWihCvYsYldPEFqogj2L2NUThBaqYM8idvUEoYUq2LOIXT1BaKEK9ixiV08QWqiCPYvY1ROEFqpgzyJ29QShhSrYs4hdPUFooQr2LGJXTxBaqII9i9jVE4QWqmDPInb1BKGFKtiziF09QWihCvYsYldPEFqogj2L2NUThRqKYM0idvVEoYYa2LKKbUVB6KEGtqxiW1EQeqiBLavYVhSEHmpgyyq2FUWhiArYsYx9TUFoogJ2LGNfUxCaqIAdy9jXFIUqCmDFMvY1RaGKediwjo1VQehiHjasY2NVFMqYhfkL2dkVhDZmYf5CdnZFoY45mL6SrWVB6GMOpq9ka1kUCpmB2UvZ2xaFSvIweS2b64LQSR4mr2VzXRRKycLcxezui0ItOZi6mu2FQeglB1NXs70wCsVkYOZy9jdGoRofJq7nhsoolOPCvA3c0RmFejyYtoNbSqNQkAOztnBPaxQqisOkPdxUG4WSgjBmF7cVR6GoCMzYxn3NUahqDBP2cWN1GNr6DF/v5NbyMDR2DV/u5eb6MLR2Dl/t5vYDwtDcO3yxny84IQ71HeHuLXzHFXEo8Qdu3cXXHGLyXRZ/+bqD/mnaZiVts5J/w+b4X8nhwhbuvyDA8D/dw4VN3H5AhF9ZV8eOvm/j9gOOXDl52Tr5Jp8vvvDPy9jXNOTSifwvO799+nj9ZRGbaoYcfvj7T//08cM3+cSPS9jRMQQ/++2Hf/h6/YXf3r/Xs7xgDH/yf3zeGH84+/hANspZHD+Ev/bJYGnw9/OPvxyXilmZPYY/9C+jtcu/fnr04rhXybLgMfyJymjz/I+f3xw4bBayKHYMf94bzu4vxpNDfB1rUofwt53gbf9gPTkUVLEicwh/1znu/vEJP5zyt6CKBZEj+KMuWf/itV9EeeAQ/qJr/CfpB0VU5w3h7/mE/cbdr9ZZHDeEv+Yz148u/3qxf8nhtnlq04bwt4w4f3UZd/7Xz7zCCigNG8JfMuTk2afI978EQOAMlVlD+DsC4B0Tf7jajsG0CSqz4r/a4PCQcUe468CsPJVRowtlIQxTBvB5BGakKUvihe/BXAjCmAF8HoIhWaqCeN/JifwehTkf4eMYTMlSFMTzHgRWQmjOZ/g2CGOSFOXwugeBlRCa8xE+jcKcJDU5PO5JYCXEMecjfBiGQUlqcnjck8BKjGPQJ/guDpNylMTwtF8iOzGOSdfwlQGjcpTE8LQXkZ0Qh6Br+MiBWTlKYnjai8BKkGPdFXzjwKwcJTE87UVgJcqx7xy+sGBYjpIYnvYisBLmWHgG9z2YlqMkhqe9CKyEORaewX0PpuUoieFpLwIrcY6N73DbhHE5SmJ42i+BFYdD3DtcNmFcjpoY3vZkvGFxiHuDuy7My1ETw9uejDc8DnmAmzYMzFESw9OeBFZMjokCF20YmKMkhqc9Cay4HCMPcM2HiTkqYnjZk8iOi2S+4FYCRuYoiOFhT0JLNpr6hEsJGJmjIIaH/RBaSoDcqmxm5piP4V0PuHS+lYHBRdEMzTEfE7uLW3mYXBPN0BzTMcGzuDbBkmSEJpmOCV6FtSlWJGtmltmY6FXcm2FFsGSmmY2JHoW9ORbkHiInmIwJH8XFB1w6wl0huGZwrM4zGRO+CYsPuKNwWwgtOWh3lrmY+EnYfMAdhdtCZMcC5UnmYuInYXOw/YD7QmDFguU55mKMi3R1uH764sh4w4LlOaZinIuwO1o/fXFkvGHB8hxTMc5BujveP3shDBcsWJ5jJsY6CMupBwthe46ZGOseXQ684PZa2J5jJsa6R5efcOkv3FwN+3PMxFj36PLXwXNzTMR45+j218Fzc0zEeOfo9tfBc3NMxHjn6PbXwXNzTMR418j298Fzc0zEeNfI9jzVoTw3Rz7GvEbXJ6mPPZ6aJx9jXqPrc/zmFeby3Bz5GPMYWZ/jkFYWzHNz5GPMY2R9jmNYVTDPzZGPMY+R9SkkrCqZ5+bIx5jHyPoUmlWUzHNz5GPMY2R9irZZ9JsftM2i3/ygbRb95gcSVpXMc3PkY9xjZH8KtXn8kofX5sjHuMfI/hSHsLJcXpsjH+MeI/tz/MYVxvLaHPkY9xjZn6Q+9XhpnnyMe4zsT1OdyWtz5GPcY2T/6+C1OfIx7jGy/3Xw2hz5GPcY2f86eG2OfIx7je5/G7w2Rz7GvUb3vw1em2MixrxG1r8OXptjIsY8R9e/DV6b4yKGZQ2hsQenf+XL5gxau7DJZ8059HZqk4+aK2juxCafNNe8uXv7Q2NAeZjbpgXlcW4sYE/HtmkCezq2TRPY07FtmsCejm3TRe3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQ2bdSeTG3TRu3J1DZt1J5MbdNG7cnUNm3Unkxt00btydQybVSfTK3TBfZ0bJsmsKdj2zSBPR3bpgns6dg2TWBPx9bpQXmY26YF5WFunQ5v7viH1hmH5s5sts4g9HZus3WGoLUrm+1zDI09OP/rA75vfqGpF9dfGp+2WUnbrKRtVtI2K2mblbTNStpmJW2zkrZZSduspG1W8n9Nu8VENIkQWgAAAABJRU5ErkJggg=="
                        )
                    ]);

                    ctx.drawImage(qrImg, 295, 10, size, size);
                    ctx.drawImage(logoImg, 5, 145, 90, 90);
                } catch (err) {
                    console.error("Image load failed", err);
                }

                ctx.font = "19px monospace";
                ctx.fillText(order.hash, 298, 105);
                this.drawWrappedText(
                    ctx,
                    "Scan the QR Code to open the Quickstart Guide",
                    "20px Inter",
                    "20px Inter",
                    110,
                    170,
                    lineHeight,
                    0,
                    "",
                    260
                );
            };

            if (type === "standard_sender") {
                ctx.font = "32px Inter";
                ctx.fillText("Florian Heß", x, y);
                y += lineHeight;
                ctx.fillText("Petrinistr. 19", x, y);
                y += lineHeight;
                ctx.fillText("97080 Würzburg", x, y);
                y += lineHeight + 20;
                await drawQRCodeAndLogo(order.hash);

            } else if (type === "quickstart") {
                y = this.drawWrappedText(ctx, "Thank you", normalFont, normalFont, x, y, lineHeight, 0, "", 260);
                y = this.drawWrappedText(ctx, `for supporting the Doorman project, ${order.name.split(" ")[0]}.`, "20px Inter", "20px Inter", x, y, lineHeight, 0, "", 270);
                await drawQRCodeAndLogo(order.hash);

            } else if (type === "standard_receiver") {
                y = this.drawWrappedText(ctx, order.name, smallFont, smallFont, x, y, lineHeight, 0, "-", maxWidth);
                if (order.address_extra) {
                    y = this.drawWrappedText(ctx, order.address_extra, smallFont, smallFont, x, y, lineHeight, 0, "-", maxWidth);
                }
                y = this.drawWrappedText(ctx, order.street, smallFont, smallFont, x, y, lineHeight, 0, "-", maxWidth);
                y = this.drawWrappedText(ctx, `${order.zip} ${order.city}`, smallFont, smallFont, x, y, lineHeight, 0, "", maxWidth);
            }

            return true;
        },
        drawWrappedText(ctx, text, font, smallFont, x, y, lineHeight, offset, divider, maxWidth) {
            ctx.font = font;
            const words = text.split(' ');
            let line = '';
            let lines = [];
            
            // Precompute wrapping
            for (let n = 0; n < words.length; n++) {
                const testLine = line + words[n] + ' ';
                const testWidth = ctx.measureText(testLine).width;
                if (testWidth > maxWidth && n > 0) {
                    lines.push(line.trim());
                    line = words[n] + ' ';
                } else {
                    line = testLine;
                }
            }
            lines.push(line.trim());

            // If wrapped to multiple lines → use smaller font
            const isWrapped = lines.length > 1;
            if (isWrapped) {
                ctx.font = smallFont;
            }

            y = y + offset;

            // Draw each line
            lines.forEach((l, i) => {
                if (i < lines.length - 1) {
                    ctx.fillText(isWrapped ? (l + divider) : l, x, y);
                    // Only add smaller spacing between wrapped lines
                    y += isWrapped ? lineHeight - 10 : lineHeight;
                }
                else {
                    ctx.fillText(l, x, y);
                }
            });

            // After last line, always add normal spacing
            y += lineHeight;

            return y;
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
    flex: 1 1 50px;
    min-width: 200px;
}

/* BUTTONS */
.order_row .header .actions button {
    flex: 1 1 0;
    min-width: 80px;
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

.order_row .details .full-row {
    flex: 1 1 100%;
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

canvas {
    border: 1px solid gray;
    border-radius: 8px;
}
</style>

<ContactModal :show="settingsOpen" @close="settingsOpen = false">
    <template #header>
        <h3>Settings</h3>
    </template>
    <template #body>
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
                <h5 class="firmware_title_row" style="margin-bottom: 5px;margin-top: 0;" @click="printerDebug = !printerDebug">Printer</h5>
                <div>Manage the Label Printer connection.</div>
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
                <VPButton style="width: 100%" type="button" @click="connectBLE" v-if="!printerClientIsConnected && !printerClientIsConnecting" text="BLE" />
                <VPButton style="width: 100%" type="button" @click="connectSerial" v-if="!printerClientIsConnected && !printerClientIsConnecting" text="Serial" />
                <VPButton style="width: 100%" type="button" @click="disconnect" v-if="printerClientIsConnected" text="Disconnect" />
                <div v-if="printerClientIsConnecting" class="tip custom-block" style="width: 100%;">
                    <p class="custom-block-title">CONNECTING</p>
                    <p>Waiting for printer connection...</p>
                </div>
            </div>
        </div>
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
    </template>
</ContactModal>

<ContactModal :show="modalOpen" @close="modalOpen = false">
    <template #header>
        <h3>{{ result_title }}</h3>
    </template>
    <template #body>
        {{ result_text }}
    </template>
</ContactModal>

<div style="display: flex;justify-content: space-between;align-items:center;">
    <h1 class="firmware_title_row" style="margin-bottom: 10px;margin-top: 10px;">Order Management <Badge v-if="printerClientIsConnected" type="tip" text="Connected" /></h1>
    <div v-if="user" style="display: flex; gap: 15px;">
        <VPButton theme="alt" type="button" text="⚙️" @click="settingsOpen = !settingsOpen" />
        <VPButton theme="alt" type="button" text="Logout" @click="logout" />
    </div>
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
    <div v-show="printerDebug" >
        <h5 class="firmware_title_row" style="margin-bottom: 5px;margin-top: 0px;">Print Debugging</h5>
        <canvas ref="labelCanvas" width="384" height="240"></canvas>
        <br>
        <VPButton type="button" @click="print" v-if="printerClientIsConnected" text="Print" />
        <VPButton theme="alt" type="button" @click="printerDebug = false" text="Close" />
        <hr>
    </div>
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
        <div v-for="order in paginatedOrders" :key="order.id" class="order_row" @click="generateLabel(order, 'quickstart')">
            <div class="header">
                <div class="title">
                    <div class="name">ORDER FROM {{ order.name.split(' ')[0].toUpperCase() }} <Badge :type="statusLabelColor(order.status)">{{ statusLabel(order.status) }}</Badge></div>
                    <div class="meta">
                        <span><MingcuteTimeFill /> {{ moment.unix(order.timestamp).format('DD.MM.YYYY') }}</span>
                        <span v-if="order.email"><IonMail /> {{ order.email }}</span>
                        <span v-if="order.discord"><IcBaselineDiscord /> {{ order.discord }}</span>
                        <span v-if="order.model"><PhCpuBold /> {{ order.model }}</span>
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
                <div v-if="order.message" class="section" :class="{ 'full-row': order.message.length > 100 }">
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
from bargainflow.forms import CommissionForm, CommissionBidForm
from bargainflow.models import Commission, CommissionBid
from cc.models import Wallet
from cc.tasks import process_withdraw_transactions, query_transactions
from django.contrib.auth.decorators import login_required
from django.contrib.auth.models import User
from django.db.models import Q
from django.shortcuts import redirect, get_object_or_404, reverse
from django.utils.decorators import method_decorator
from django.views.generic import TemplateView
from django.views.generic.detail import DetailView
from django.views.generic.edit import FormView
from django.views.generic.list import ListView
from moneyflow.forms import WithdrawForm
from moneyflow.models import Customer
from opinions.forms import OpinionAddForm
from opinions.models import Opinion
from registration.backends.hmac.views import RegistrationView


class RegisterView(RegistrationView):
    success_url = "/"


class LoginView(TemplateView):
    template_name = "registration/login.html"


class HomeView(TemplateView):
    template_name = "base/main.html"


class CommissionDashboardView(ListView):
    template_name = "base/commission_list.html"
    model = Commission
    context_object_name = "comm_list"
    paginate_by = 10

    def get_queryset(self):
        queryset = Commission.objects.all()
        desc = self.request.GET.get('desc', None)
        if desc is not None:
            queryset = queryset.filter(Q(description__icontains=desc) |
                                       Q(title__iexact=desc) |
                                       Q(tags__name__in=[desc]))

        return queryset.distinct()


@method_decorator(login_required, name='dispatch')
class CommissionUserView(ListView):
    template_name = "base/commission_user.html"
    model = Commission
    context_object_name = "comm_user"
    paginate_by = 10

    def get_queryset(self):
        queryset = Commission.objects.all()
        pk = self.request.GET.get('pk', None)
        if pk is not None:
            queryset = queryset.filter(Q(orderer=pk))
        else:
            queryset = Commission.objects.none()

        return queryset.distinct()


@method_decorator(login_required, name='dispatch')
class CommissionUserBiddedView(ListView):
    template_name = "base/commission_user_bidded.html"
    model = Commission
    context_object_name = "comm_user"
    paginate_by = 10

    def get_queryset(self):
        bidded = CommissionBid.objects.all()
        pk = self.request.GET.get('pk', None)
        if pk is not None:
            bidded = bidded.filter(bidder=pk)
        else:
            bidded = CommissionBid.objects.none()

        bidded_pk = []
        for x in bidded:
            bidded_pk.append(x.commission.pk)

        queryset = Commission.objects.all().filter(pk__in=bidded_pk)

        return queryset.distinct()

    def get_context_data(self, **kwargs):
        context = super(CommissionUserBiddedView, self).get_context_data(**kwargs)
        pk = self.request.GET.get('pk', None)
        context['user_name'] = User.objects.all().filter(pk=pk)[0].username
        return context


@method_decorator(login_required, name='dispatch')
class CommissionUserToOpinionView(ListView):
    template_name = "base/commission_user_opinion.html"
    model = Commission
    context_object_name = "comm_user"
    paginate_by = 10

    def get_queryset(self):
        queryset = Commission.objects.all()
        pk = self.request.GET.get('pk', None)
        if pk is not None:
            queryset = queryset.filter(contractor=pk).filter(status='F')
        else:
            queryset = Commission.objects.none()

        return queryset.distinct()

    def get_context_data(self, **kwargs):
        context = super(CommissionUserToOpinionView, self).get_context_data(**kwargs)
        pk = self.request.GET.get('pk', None)
        context['user_name'] = User.objects.all().filter(pk=pk)[0].username
        return context


@method_decorator(login_required, name='dispatch')
class CommissionView(DetailView):
    model = Commission
    template_name = "base/commission_detail.html"

    def get_context_data(self, **kwargs):
        context = super(CommissionView, self).get_context_data(**kwargs)
        context['commission_bids'] = context['object'].commission_bids
        commission = self.get_object()
        context['form'] = self._get_commission_bid_form()

        user = self.request.user
        if (user == commission.orderer or user == commission.contractor):
            context['involved'] = True
            if (commission in Opinion.list_possible_give(user)):
                context['opinion_give'] = True

        if commission.contractor:
            context['chosen_bid'] = commission.commissionbid_set.get(bidder=commission.contractor)
        return context

    def _get_commission_bid_form(self):
        commission = self.get_object()
        user_bid = commission.commissionbid_set.filter(bidder=self.request.user)
        if user_bid.exists():
            return CommissionBidForm(instance=user_bid.first())
        else:
            return CommissionBidForm(initial={'commission': commission})


def commission_choose(request, pk, bid_id):
    commission = get_object_or_404(Commission, pk=pk)
    commission_bid = get_object_or_404(CommissionBid, pk=bid_id, commission=commission)
    commission.contractor = commission_bid.bidder
    commission.bid = commission_bid
    commission.status = 'A'
    commission.save()
    return redirect('commission-detail', pk=pk)


@method_decorator(login_required, name='dispatch')
class CommissionBidView(FormView):
    form_class = CommissionBidForm

    def __init__(self):
        self.commission = None
        super(CommissionBidView, self).__init__()

    def get_success_url(self):
        return reverse('commission-detail', kwargs={'pk': self.commission.id})

    def form_valid(self, form):
        commission_bid = form.save(commit=False)
        commission_bid.commission.status = 'B'
        commission_bid.bidder = self.request.user
        commission_bid.save()
        self.commission = commission_bid.commission
        return super(CommissionBidView, self).form_valid(form)


@method_decorator(login_required, name='dispatch')
class CommissionAddView(FormView):
    template_name = "base/commission_add.html"
    form_class = CommissionForm

    def __init__(self):
        self.commission = None
        super(CommissionAddView, self).__init__()

    def get_success_url(self):
        return reverse('commission-detail', kwargs={'pk': self.commission.id})

    def form_valid(self, form):
        commission = form.save(commit=False)
        commission.orderer = self.request.user
        commission.contractor = None
        master_wallet = get_object_or_404(Wallet, label='master_wallet')
        print(commission.price.__class__)
        self.request.user.user_ext.wallet.transfer(commission.price, master_wallet)
        commission.save()
        self.commission = commission
        form.save_m2m()
        return super(CommissionAddView, self).form_valid(form)


def commission_accept_work(request, commission_id):
    commission = get_object_or_404(Commission, pk=commission_id)
    if commission.status == 'F':
        redirect('commission-detail', pk=commission_id)

    commission.status = 'F'
    master_wallet = Wallet.objects.get(label='master_wallet')
    master_wallet.transfer(commission.price, commission.contractor.user_ext.wallet)
    commission.save()
    return redirect('commission-detail', pk=commission_id)


def commission_done(request, commission_id):
    commission = get_object_or_404(Commission, pk=commission_id)
    commission.status = 'D'
    commission.save()
    return redirect('commission-detail', pk=commission_id)


def commission_deny(request, commission_id):
    commission = get_object_or_404(Commission, pk=commission_id)
    commission.status = 'B'
    commission.contractor = None
    commission.save()
    return redirect('commission-detail', pk=commission_id)


class CustomerView(DetailView):
    model = Customer
    template_name = "base/customer_detail.html"

    def get_context_data(self, **kwargs):
        context = super(CustomerView, self).get_context_data(**kwargs)
        return context


@method_decorator(login_required, name='dispatch')
class OpinionAddView(FormView):
    template_name = "opinions/add_opinion.html"
    form_class = OpinionAddForm

    def __init__(self):
        self.opinion = None
        self.pk = 0
        super(OpinionAddView, self).__init__()

    def get_success_url(self, **kwargs):
        return reverse('commission-detail', kwargs={'pk': self.pk})

    def form_valid(self, form):
        self.pk = self.request.GET['pk']
        opinion = form.save(commit=False)
        opinion.opinion_giver = self.request.user
        opinion.commission = Commission.objects.get(pk=self.pk)
        opinion.save()
        self.opinion = opinion
        form.save_m2m()
        return super(OpinionAddView, self).form_valid(form)

    def get_context_data(self, **kwargs):
        context = super(OpinionAddView, self).get_context_data(**kwargs)
        context['pk'] = self.request.GET['pk']
        return context



class OpinionUserView(ListView):
    template_name = "opinions/customer_opinions.html"
    model = Opinion
    context_object_name = "opinion_list"
    paginate_by = 10

    def get_context_data(self, **kwargs):
        context = super(OpinionUserView, self).get_context_data(**kwargs)
        context['ratings'] = Opinion.RATINGS
        return context

    def get_queryset(self):
        pk = self.request.GET.get('pk', None)
        if pk is not None:
            return Opinion.list_current_about(User.objects.get(pk=pk))
        else:
            Opinion.objects.none()


class Error500View(TemplateView):
    template_name = "500.html"


class Error403View(TemplateView):
    template_name = "403.html"


class Error404View(TemplateView):
    template_name = "404.html"


@method_decorator(login_required, name='dispatch')
class WalletView(FormView):
    template_name = 'base/wallet.html'
    form_class = WithdrawForm

    def form_valid(self, form):
        wallet = self.request.user.user_ext.wallet
        try:
            wallet.withdraw(form.cleaned_data['amount'], form.cleaned_data['address'])
        except ValueError as e:
            form.add_error(e)
            return self.form_valid(form)

        process_withdraw_transactions(ticker=wallet.currency.ticker)

        return super(WalletView, self).form_valid(form)

    def get_success_url(self):
        return reverse('wallet_detail')

    def get_context_data(self, **kwargs):
        context = super(WalletView, self).get_context_data();
        context['wallet'] = self.request.user.user_ext.wallet
        return context


def check_deposit(request):
    query_transactions()
    request.user.user_ext.wallet.recalc_balance(True)
    return redirect('wallet-details')
